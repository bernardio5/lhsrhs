#pragma once

// # seq rows 
#define SEQ_ROW_CT 18

// length of row displays in chars
#define SEQ_COL_CT 56  

// tick per measure/ #chars in seq string
#define SEQ_STEPS 16   

// control count per row
#define SEQ_CTRL_CT 8

// 1st col of RHS-- limits cursor mvmt
#define SEQ_RT_START 28 
// seq_Rt_Start =  =steps + 4 + mod_ct


#define SEQ_SAMPLE_SPEED 44100

// instrument enum-ish
#define SEQ_INST_NULL '0'

// mine
#define SEQ_IN_DRUM '1'
#define SEQ_IN_SINE '2'   
#define SEQ_IN_SQUARE '3'
#define SEQ_IN_SAW '4'
#define SEQ_IN_NOISE '5'

// theirs, added, working-ish
#define SEQ_IN_BAR '6' 		// Uniform Bar (bandedWG)
#define SEQ_IN_TUNEDBAR '7'	// Tuned Bar (bandedWG)
#define SEQ_IN_GLASSHARM '8'  // Glass Harmonica (bandedWG)
#define SEQ_IN_BOWL '9' 	// Tibetan Bowl (bandedWG)

// children of FM, generally all the same parameters
#define SEQ_IN_B3 'a' 		// BeeThree
#define SEQ_IN_FMVOICES 'b' // FMVoices
#define SEQ_IN_RHODEY 'c' 	// Rhodey
#define SEQ_IN_TUBEBELL 'd' // TubeBell
#define SEQ_IN_WURLEY 'e' 	// Wurley

#define SEQ_IN_SIMPLE 'z' 	// Simple

/*
// TBD
#define SEQ_IN_BLOW 'b'   	// BlowBotl
#define SEQ_IN_BOWED 'c' 	// Bowed
#define SEQ_IN_FLUTE 'd' 	// Flute
#define SEQ_IN_PLUCKED 'e' 	// Plucked
#define SEQ_IN_SHAKERS 'f' 	// Shakers
#define SEQ_IN_SITAR 'h' 	// Sitar
#define SEQ_IN_STIF 'i' 	// StifKarp
#define SEQ_IN_VOICE 'j' 	// VoicForm
#define SEQ_IN_WHISTLE 'k' 	// Whistle
*/


/* instruments and things that I'm not seeing in STK
2) overdrive filters
3) bitcrushing
4) sample and hold 
5) tuning/clamping
7) gnarly sample playback things. 
9) filter objects that redirect things already connected to the mixer

The 0-128 range for controls is MIDI throwback. 
Interferes with using stuff for controls? 
Relax, it's one multiply at control rate. 
And you'd probably have to scale things anyway. 


*/




