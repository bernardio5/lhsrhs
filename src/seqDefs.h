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
#define SEQ_IN_CONGA '2'
#define SEQ_IN_TABLA '3'
#define SEQ_IN_SYNDRUM '4'

#define SEQ_IN_SINE '5'   
#define SEQ_IN_SQUARE '6'
#define SEQ_IN_SAW '7'
#define SEQ_IN_NOISE '8'

// theirs, added, working-ish
#define SEQ_IN_BAR 'a' 		// Uniform Bar (bandedWG)
#define SEQ_IN_TUNEDBAR 'b'	// Tuned Bar (bandedWG)
#define SEQ_IN_GLASSHARM 'c'  // Glass Harmonica (bandedWG)
#define SEQ_IN_BOWL 'd' 	// Tibetan Bowl (bandedWG)

// children of FM, generally all the same parameters
#define SEQ_IN_B3 'e' 		// BeeThree
#define SEQ_IN_FMVOICES 'f' // FMVoices
#define SEQ_IN_RHODEY 'g' 	// Rhodey
#define SEQ_IN_TUBEBELL 'h' // TubeBell
#define SEQ_IN_WURLEY 'i' 	// Wurley

// FM's of the 8 standard algorithms   #4 always feeds back into itself
#define SEQ_FM_1 'n'		// 4->3->2->1->out
#define SEQ_FM_2 'o'		// 4->2, 3->2->1->out
#define SEQ_FM_3 'p'		// 4->1, 3->2->1->out
#define SEQ_FM_4 'q'		// 2->1->out, 4->3->1
#define SEQ_FM_5 'r'		// 2->1->out, 4->3->out
#define SEQ_FM_6 's'		// 4->1->out, 4->2->out, 4->3->out
#define SEQ_FM_7 't'		// 4->3->out, 1->out, 2->out
#define SEQ_FM_8 'u'		// 1->out, 2->out, 3->out, 4->out

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



