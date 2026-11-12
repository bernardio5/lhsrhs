#pragma once

#include <curses.h>
#include <cstring>
#include <stdlib.h>

#include "seqDefs.h"
#include "seqMet.h"
#include "seqOrca.h"
#include "seqKeyable.h"

  /* for each row, each character has a different use
  0-15: not used
  16: timer: if non-0, is the number of measures until this line's data is applied
  17: lerp time: if non-0, the # meas in the lerps of mix, a,b,c
  19,20,21: editable digits of BPM-- not orca! weird.
  25,26: editable global reverb and delay

  44,45,46: current bpm
  50,51: current reverb and delay
  other chars not used. 

  bpm is stored in theMet. it is displayed using the pair of 3 digits. 
  but it is lerped by one orca, "bpm", below-- theMet doesn't lerp BPM.
  The orca must have it in range [0,1], so mult by 360. 
  */

class seqTimeRow {

public:
    seqTimeRow() {}
    void init(seqMet *m); 

    void edit(int col, char c); 
    void update();
    void draw(WINDOW *win);
    void getHelpString(char *buffer);

    void setBPM(); // "edit" helper to munge the 3 digits to the keyable bpm

    seqOrca timer; 
    int lerpTime, hundreds, tens, ones;

    seqKeyable bpm, r, d; 
    seqMet *theMet;
    int lastStep; 
};
