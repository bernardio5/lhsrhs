#include "seqInst.h"

// my own!
#include "inDrummer2.h"
#include "inBandedWG2.h"
#include "inWaves.h"

// direct children of instrument class
#include "../include/BlowBotl.h"
#include "../include/Bowed.h"
#include "../include/Flute.h"
#include "../include/Plucked.h"
#include "../include/Shakers.h"
#include "../include/Simple.h"
#include "../include/Sitar.h"
#include "../include/StifKarp.h"
#include "../include/VoicForm.h"
#include "../include/Whistle.h"

// children of FM
#include "../include/BeeThree.h"
#include "../include/FMVoices.h"
#include "../include/Rhodey.h"
#include "../include/TubeBell.h"
#include "../include/Wurley.h"

#include "../include/SKINImsg.h"


seqInst::~seqInst() { // deallocate elements
	if (instP!=0) { delete instP; }
}


void seqInst::init(int rw, char type, seqTunerRow *t, inMixer *mx) { 
	myRow = rw; 
	myType = SEQ_INST_NULL;
	theTuner = t; 
	theMixer = mx; 
	instP = 0; 
	editType(type); 
}


void seqInst::editType(char type) {
	if (type==myType) return; 
	Instrmnt *newp = 0; 
	StkFloat tonic = (StkFloat)theTuner->freq(0);
    inBandedWG2 *b;
	switch (type) { 
		case SEQ_IN_DRUM: 		newp = new inDrummer2(); break;
	    case SEQ_IN_SINE: 			 
	    case SEQ_IN_SQUARE: 
	    case SEQ_IN_SAW: 
	    case SEQ_IN_NOISE: 		newp = new inWaves(type); break;
		case SEQ_IN_BAR: 		b = new inBandedWG2(); b->setPreset(0); newp = b; break;
		case SEQ_IN_TUNEDBAR: 	b = new inBandedWG2(); b->setPreset(1); newp = b; break;
		case SEQ_IN_GLASSHARM: 	b = new inBandedWG2(); b->setPreset(2); newp = b; break;
		case SEQ_IN_BOWL: 		b = new inBandedWG2(); b->setPreset(3); newp = b; break;
		case SEQ_IN_B3:	 		newp = new BeeThree(); break;
		case SEQ_IN_FMVOICES:	newp = new FMVoices(); break;
		case SEQ_IN_RHODEY:	 	newp = new Rhodey(); break;
		case SEQ_IN_TUBEBELL:	newp = new TubeBell(); break;
		case SEQ_IN_WURLEY:	 	newp = new Wurley(); break;
/*		case SEQ_IN_BLOW: 		newp = new BlowBotl(); break;
		case SEQ_IN_BOWED: 		newp = new Bowed(); break;
		case SEQ_IN_FLUTE: 		newp = new Flute(tonic); break;
		case SEQ_IN_PLUCKED: 	newp = new Plucked(); break;
		case SEQ_IN_SHAKERS:	newp = new Shakers(); break;
		case SEQ_IN_SITAR:	 	newp = new Sitar(); break;
		case SEQ_IN_STIF:	 	newp = new StifKarp(); break;
		case SEQ_IN_VOICE:	 	newp = new VoicForm(); break;
		case SEQ_IN_WHISTLE:	newp = new Whistle(); break;
*/		default: 				newp = new Simple(); break; 
	} 
	if (newp!=0) { 		// mutex!
		myType = type; 
		if (instP!=0) { delete instP; }
		instP = newp; 
		theMixer->setInst(myRow, instP);
	}
}


std::string seqInst::getDefaults() {
	std::string res = "12iixxxi"; 
	switch (myType) { 
		case SEQ_IN_DRUM: 		res = "xxxxxxx5"; break;  // 'x'=>unused dont care

	    case SEQ_IN_SINE: 			 
	    case SEQ_IN_SQUARE: 
	    case SEQ_IN_SAW: 
	    case SEQ_IN_NOISE: 		res = "22v2ixxh"; break;

		case SEQ_IN_BAR: 			
		case SEQ_IN_TUNEDBAR:		
		case SEQ_IN_GLASSHARM:		
		case SEQ_IN_BOWL:		res = "m00m00xw"; break; // all the same banded 

		case SEQ_IN_B3:	 			
		case SEQ_IN_RHODEY:	 		
		case SEQ_IN_TUBEBELL:	 	
		case SEQ_IN_WURLEY:	 	res = "zz2mgxxm"; 	break;
		case SEQ_IN_FMVOICES:	res = "z0mggxxm"; 	break;

		default:			 	res = "5555xxxm"; break; // simple
	}
	return res; 
}


void seqInst::trigger(char evt, seqKeyable *ctrls) {
	switch (myType) { 
		case SEQ_IN_DRUM: 		triggerAsSampDrum(evt, ctrls); break; 

	    case SEQ_IN_SINE: 			 
	    case SEQ_IN_SQUARE: 
	    case SEQ_IN_SAW: 
	    case SEQ_IN_NOISE: 		triggerAsWaves(evt, ctrls); break;

		case SEQ_IN_BAR: 
		case SEQ_IN_TUNEDBAR:	
		case SEQ_IN_GLASSHARM:	
		case SEQ_IN_BOWL:		triggerAsBanded(evt, ctrls); break;

		case SEQ_IN_B3:	 			
		case SEQ_IN_FMVOICES:	 	
		case SEQ_IN_RHODEY:	 		
		case SEQ_IN_TUBEBELL:	 	
		case SEQ_IN_WURLEY:	 	triggerAsFM(evt, ctrls); break;

		default: 				triggerAsSimple(evt, ctrls); break; 
	}
}


std::string seqInst::getInfo() {
	std::string res = "unimplemented  "; 
	switch (myType) { 					//"123456789012345"
		case SEQ_IN_DRUM: 		res = "Drummer2       "; break;

	    case SEQ_IN_SINE: 		res = "Simple/Sine    "; break; 		 
	    case SEQ_IN_SQUARE: 	res = "Simple/Square  "; break; 	
	    case SEQ_IN_SAW: 		res = "Simple/Saw     "; break; 	
	    case SEQ_IN_NOISE: 		res = "Simple/Noise   "; break; 	

		case SEQ_IN_BAR: 		res = "Uniform Bar    "; break; 	
		case SEQ_IN_TUNEDBAR:	res = "Tuned Bar      "; break;
		case SEQ_IN_GLASSHARM:	res = "Glass Harmonica"; break; 
		case SEQ_IN_BOWL:		res = "Tibetan Bowl   "; break;

		case SEQ_IN_B3:	 		res = "BeeThree       "; break;
		case SEQ_IN_FMVOICES:	res = "FMVoices       "; break;
		case SEQ_IN_RHODEY:	 	res = "Rhodey         "; break;
		case SEQ_IN_TUBEBELL:	res = "TubeBell       "; break;
		case SEQ_IN_WURLEY:	 	res = "Wurley         "; break;
/*
		case SEQ_IN_BLOW: 		res = "BlowBotl       "; break;
		case SEQ_IN_BOWED: 		res = "Bowed          "; break;
		case SEQ_IN_FLUTE: 		res = "Flute          "; break;
		case SEQ_IN_PLUCKED: 	res = "Plucked        "; break;
		case SEQ_IN_SHAKERS:	res = "Shakers        "; break;
		case SEQ_IN_SITAR:	 	res = "Sitar          "; break;
		case SEQ_IN_STIF:	 	res = "StifKarp       "; break;
		case SEQ_IN_VOICE:	 	res = "VoicForm       "; break;
		case SEQ_IN_WHISTLE:	res = "Whistle        "; break;*/

		default:	 			res = "Simple         "; break;
	} 
	return res;
}




///////////////////////////////////////////// simple (sine/square/saw/noise)
// wave generator + env + filter, loudness set by params[7], seq sets pitch, tuned


void seqInst::triggerAsSampDrum(char evt, seqKeyable *ctrls) {
	StkFloat amp = ctrls[7].val(); // should be range [0,1]
	if (evt=='.') { 
		instP->noteOff( amp );
	} else {
		float pitch = (StkFloat)(seqOrca::CtoI(evt)); 
        instP->noteOn( pitch, amp );
	}
}


void seqInst::triggerAsWaves(char evt, seqKeyable *ctrls) {
	inWaves *p = (inWaves*)instP; 
//	if (!ctrls[0].inert) { // ADSR A, seconds
		p->controlChange(0, ctrls[0].val());
//	}
//	if (!ctrls[1].inert) { // ADSR D, seconds
		instP->controlChange(1, ctrls[1].val());
//	}
//	if (!ctrls[2].inert) { // ADSR S, %
		instP->controlChange(2, ctrls[2].val());
//	}
//	if (!ctrls[3].inert) { // ADSR R, seconds
		instP->controlChange(3, ctrls[3].val());
//	}
//	if (!ctrls[4].inert) { // pole z-value
		instP->controlChange(4, (ctrls[4].val()*0.9)+0.05);
//	}
	StkFloat amp = ctrls[7].val(); // volume
	if (evt=='.') { 
		instP->noteOff( amp );
	} else {
		int deg = seqOrca::CtoI(evt); // pitch control  
		StkFloat pitch = (StkFloat)theTuner->freq(deg);
        instP->noteOn( pitch, amp );
	}
}


// children of banded
void seqInst::triggerAsBanded(char evt, seqKeyable *ctrls) {
	if (!ctrls[0].inert) { // 0
		instP->controlChange(2, 128.0 * ctrls[0].val()); // bow pressure
	}
	if (!ctrls[1].inert) { // 0
		instP->controlChange(4, 128.0 * ctrls[1].val() ); // bow motion
	}
	if (!ctrls[2].inert) { // vibrato frequency (units?)
		instP->controlChange(11, 128.0 * ctrls[2].val());
	}
	if (!ctrls[3].inert) { // gain
		instP->controlChange(1, 128.0 * ctrls[3].val()); // base gain 
	}
	if (!ctrls[4].inert) { // bow velocity
		instP->controlChange(128, 128.0 * ctrls[4].val());
	}
	if (!ctrls[5].inert) { // set striking
		instP->controlChange(64, 128.0 * ctrls[5].val());
	}
	StkFloat amp = ctrls[7].val(); // should be range [0,1]
	if (evt=='.') { 
		instP->noteOff( amp );
	} else {
		int deg = seqOrca::CtoI(evt); // pitch control  
		StkFloat pitch = (StkFloat)theTuner->freq(deg);
        instP->noteOn( pitch, amp );
	}
}



// children of FM 
void seqInst::triggerAsFM(char evt, seqKeyable *ctrls) {
	if (!ctrls[0].inert) { // 2 / op4 feedback gain / default 
		instP->controlChange(2, 128.0 * ctrls[0].val()); 
	}
	if (!ctrls[1].inert) { // 4 / op 3 gain
		instP->controlChange(4, 128.0 * ctrls[1].val() );
	}
	if (!ctrls[2].inert) { // 11 LFO speed  (expects [0,128], uses [0,1536])
		instP->controlChange(11, 128.0 * ctrls[2].val());
	}
	if (!ctrls[3].inert) { // 1 LFO "depth"
		instP->controlChange(1, 128.0 * ctrls[3].val()); // base gain 
	}
	if (!ctrls[4].inert) { // 128  ADSR 2&4 target  
		instP->controlChange(128, 128.0 * ctrls[4].val());
	}
	StkFloat amp = ctrls[7].val(); // should be range [0,1]
	if (evt=='.') { 
		instP->noteOff( amp );
	} else {
		int deg = seqOrca::CtoI(evt); // pitch control  
		StkFloat pitch = (StkFloat)theTuner->freq(deg);
        instP->noteOn( pitch, amp );
	}
}




// 
void seqInst::triggerAsSimple(char evt, seqKeyable *ctrls) {
	if (!ctrls[0].inert) { // 2 / filter pole position
		instP->controlChange(2, 128.0 * ctrls[0].val()); 
	}
	if (!ctrls[1].inert) { // 4 / crossfade
		instP->controlChange(4, 128.0 * ctrls[1].val() );
	}
	if (!ctrls[2].inert) { // 11 / envelope rate
		instP->controlChange(11, 128.0 * ctrls[2].val());
	}
	if (!ctrls[3].inert) { // 128 gain
		instP->controlChange(1, 128.0 * ctrls[3].val()); // base gain 
	}
	StkFloat amp = ctrls[7].val(); // should be range [0,1]
	if (evt=='.') { 
		instP->noteOff( amp );
	} else {
		int deg = seqOrca::CtoI(evt); // pitch control  
		StkFloat pitch = (StkFloat)theTuner->freq(deg);
        instP->noteOn( pitch, amp );
	}
}
