#pragma once

#include "seqTunerRow.h"

// stk/sound-related
#define __OS_LINUX__ 1
#include "../include/Instrmnt.h"
#include "../include/RtAudio.h"
#include "inMixer.h"
#include <math.h>
#include <algorithm>
#include <cstdlib>
using std::min;
using namespace stk;


class seqInst {

// playable instruments to be attached/owned by rows
// sequencer events cause, first, parameters to be copied over and sent: 8 floats
// then the sequencer event, which gives one orcaChar (0-9 a-z)

// init is called only from row.init.
// set & trigger are called only from row.update when a new beat brings a seq event
// getInfo is all the content in row.getInfo
// all other fns are helpers for the 1st 4 & are only called by them.
    

public:
    seqInst() {}
    ~seqInst();

    void init(int myRow, char type, seqTunerRow *t, inMixer *mx);
    Instrmnt *getInstp() { return instP; }
    void editType(char c); 
    std::string getDefaults(); // returns 8 chars that are good defaults for the current inst 
    void trigger(char evt, seqKeyable *ctrls);       
    std::string getInfo(); 

    char myType; 
    int myRow;
    seqTunerRow *theTuner;
    Instrmnt *instP; 
    inMixer *theMixer;
    
    void triggerAsSampDrum(char evt, seqKeyable *ctrls);
    void triggerAsWaves(char evt, seqKeyable *ctrls);
    void triggerAsBanded(char evt, seqKeyable *ctrls);
    void triggerAsFM(char evt, seqKeyable *ctrls);

    void triggerAsSimple(char evt, seqKeyable *ctrls);
    
};

