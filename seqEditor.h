#pragma once

#include <curses.h>
#include <cstring>
#include <stdlib.h>
#include <string>

#include "seqDefs.h"
#include "seqMet.h"
#include "seqRow.h"
#include "seqTimeRow.h"
#include "seqTunerRow.h"

// stk/sound-related
#define __OS_LINUX__ 1
#include "../include/RtAudio.h"
//#include "../include/Messager.h"
#include "inMixer.h"
//#include "../include/SKINImsg.h"
#include <math.h>
#include <algorithm>
#include <cstdlib>
#include <stdio.h>

using std::min;
using namespace stk;
// other libs to look at: FMod, ?? 

// The TickData structure holds all the class instances and data that
// are shared by the various processing functions.
//     man, I need time from this. 
struct TickData {
    //Voicer voicer;
    inMixer mixer; 
    double streamTime; 
//    int std;
    int counter;
    char errMsg[80];
};


#define DELTA_CONTROL_TICKS 64 
// default sample frames between control input checks
 

class seqEditor {
public:
    seqEditor() { }
    void init(); 
    void draw(WINDOW *win);
    void update();  
    void keyPressed(int key); 

    int caretX, caretY;
    bool keepOn; // not whether to quit

    static int ticker( void *outputBuffer, void *inputBuffer, 
        unsigned int nBufferFrames, double streamTime, 
        RtAudioStreamStatus status, void *userData );
    static void timeUpdate(double t); 
    static void processMessage( TickData* data );
    void abort(int source, bool streamToo);

private:
    // objs updated 
    static seqMet theMet;  
    static seqTimeRow timeRow; 
    static seqRow seqRows[SEQ_ROW_CT];
    static seqTunerRow tunerRow; 

    char helpString[200], debugString[79], cutSeq[SEQ_STEPS], cutSettings[8];  
    char editString[80]; 

    // keypress helper fns
    void directEdit(int row, int col, char c);
    void cutRow(int row, int col);
    void copyRow(int row, int col);
    void pasteRow(int row, int col);
    void undoRow();
    void increment(int row, int col); 
    void decrement(int row, int col); 

    TickData data;
    RtAudio dac;
    RtAudio::StreamParameters parameters;

    FILE *logfile;
};



