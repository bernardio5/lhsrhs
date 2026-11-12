#pragma once


#include <curses.h>
#include <cstring>
#include <stdlib.h>

#include "seqDefs.h"
#include "seqMet.h"
#include "seqOrca.h"
#include "seqKeyable.h"

#define SEQ_MAX_SCALE_SZ 8


  /* for each row, each character has a different use
  0: timer: if non-0, is the number of measures until this line's data is applied
  3,4: editable tonic midi
  6,7,8,9, 10,11,12,13, 14,15,16: editable semitones of tuning

  28-41: current tonic and tuning
  
  Tuning is specified by giving a list of integers
  The tonic is a MIDI number-- 12 is C1; you want it pretty low.
  The list of integers tell which (MIDI) notes are in a scale;
  C Major would be 12, 0 2 4 5 7 9 11.
  You can specify up to 11 notes, which would be all of them. 

  Orca's don't hold blanks (different from Orca's orcas!),
  so 0=>null entry. 0 (or 12) would be the octave, which is always included.
  0's are ignored, and tonics are always implicitly included. 
  So "24579b00000" is the major scale, but so is "00024579b00", works same.
  Black keys only could be "25790000000"
  Some tunings are different for going up vs going down: not supported!

  To use the tuning, pick a note by degree, "n"
  tune(n) returns a MIDI note that is the nth note in the tuning up from the tonic.
  n<0 returns the tonic
  freq(n) is the corresponding frequency (w/ "just" tempering, using 440*2^(n-37/12)) 
  Orcas go 0-35, which, for a 7-note scale, is 5 octaves. 's ok.
  You could make control 0 the "oct up/down" control. If you wanted to.

    // intrusive thoughts: 1) 0 never needed; tonic is given
    // 2) entries >11 would be weird and weird can be good
    // 3) should have 11 or more entries for the sake of obsessive compulsion
    // 4) how to denote null entries? 0? 4a) double entries? weird
    // 5) specifying the scale out-of-order will mess up yr chords if u care
    // 6) if I'm thinking about this so much it must be a fun feature


changes:
  1 live; no RHS
  2 an additional tonic modifier in a seq
  3 slow
  4 no controls; store the scale in the "controls" part
  */

class seqTunerRow {

public:
    seqTunerRow() {}
    void init(seqMet *m); 

    void edit(int col, char c); 
    void update();
    void draw(WINDOW *win);
    void getHelpString(char *buffer);


    int tune(int deg); // midi note from deg
    float freq(int deg); // frequency from degree

    void setTonic(); // helper; sets "editTonic" from 2 digits of midi tonic

    int pace, step, lastStep, measureStart;
    int tens, ones, tonic, modTonic; // not lerped, but do time the copy-over
    char scale[SEQ_MAX_SCALE_SZ], seq[SEQ_STEPS]; 
    seqMet *theMet;
};
