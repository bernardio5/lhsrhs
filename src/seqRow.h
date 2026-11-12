#pragma once

#include <curses.h>
#include <cstring>
#include <stdlib.h>

#include "seqKeyable.h"
#include "seqMet.h"
#include "seqOrca.h"
#include "seqInst.h"
#include "seqTunerRow.h"

class seqRow {
/* for each row, each character has a different use
  0-15:   editable sequencer string. Note len=SEQ_STEPS=16   +---+---+---+--- 
  16: timer: if non-0, is the number of measures until this line's data is applied
  17: lerp time: if non-0, the # meas in the lerps of mix, a,b,c
  18: instrument selector-- sets and copies, doesn't lerp
  19: pace: the number of metronome ticks per seq step for this row
  20-27: instrument settings, which lerp. See seqInst for specifics 
    usually, 20,21,22,23: ADSR, and 43: mix/vol

  28: first col of uneditable RHS. Also, current instrument
  29: pace
  30-45: current sequencer string -- remaining values are not editable
  46-54: current instrument settings

Rows are connected to instruments.
Rows generate one event per update/seq step. Want more? Got a lot of rows. 
*/

public:
    seqRow() {}
    void init(int rw, seqMet *m, seqTunerRow *t, inMixer *mx,
      char whichInst, char *initSeq, int lv);

    void edit(int col, char c); 
    void update();
    void draw(WINDOW *win, int row);
    void getHelpString(char *buffer);
    Instrmnt *getInstp() { return myInst.instP; } // oo hiding

    int myRow;
    seqMet *theMet;
    seqInst myInst;
    long myNote; // # returned by voicer::noteOn 

    // if active, all edits are immediately copied from editSeq to playingSeq
    // if not, edits are copied only when timer goes to 0.
    int isLive;

    // timer counts down till edits are applied
    // lerpTime is time spent lerping edits for keyables. 
    seqOrca timer; 

    // the editSeq is for tweaking, playingSeq is what's playing
    char lerpTime, editInst, editSeq[SEQ_STEPS], playingSeq[SEQ_STEPS]; 
    char cutSeq[SEQ_STEPS];

    int pace, editPace; // slowdown factor: 4 is default, 2 is double spd, 8 is half 
    int step, lastStep; 
    int measureStart; // seqMet->tick when the current measure started. 
    double lastEvtT; 

    // each row can have filters or other synth parameters. 
    seqKeyable keys[SEQ_CTRL_CT]; 
    char debugMsg[80];

    void cut(int col, char *cutSeq, char *cutSettings);
    void copy(int col, char *cutSeq, char *cutSettings);
    void paste(int col, char *cutSeq, char *cutSettings);
    void increment(int col);
    void decrement(int col); 

};
