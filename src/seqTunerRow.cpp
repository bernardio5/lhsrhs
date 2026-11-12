#include "seqTunerRow.h"
#include <cmath>


void seqTunerRow::init(seqMet *m) {
    theMet = m;
    pace = 1; // default 16 ticks/beat
    step = 0;
    lastStep = -1;
    measureStart = 0; 
    tens = 4;
    ones = 8;
    tonic = 48; // default is C3
    modTonic = 48;
    for (int i=0; i<SEQ_STEPS; ++i) { seq[i] = ' '; }
    for (int i=0; i<SEQ_MAX_SCALE_SZ; ++i) { scale[i] = ' '; }
    scale[0] = '2';
    scale[1] = '4';
    scale[2] = '5';
    scale[3] = '7';
    scale[4] = '9';
    scale[5] = 'b'; // that's a major scale
}


void seqTunerRow::setTonic() {
    int n = (tens *10) + ones;
    tonic = (float)n;
}


void seqTunerRow::edit(int col, char ch) {
           if (col==SEQ_STEPS+1) {
        tens = seqOrca::CtoI(ch);
        setTonic();
    } else if (col==SEQ_STEPS+2) {
        ones = seqOrca::CtoI(ch); 
        setTonic();
    } else if (col==SEQ_STEPS+3) {
        pace = seqOrca::CtoI(ch); 
    } else if (col<SEQ_STEPS) {
        seq[col] = ch;
    } else if (col>(SEQ_STEPS+3)) {
        scale[col-(SEQ_STEPS+4)] = ch;
    }
}

    
void seqTunerRow::draw(WINDOW *win) { 
    char buffer[SEQ_COL_CT];
    for (int i=0; i<SEQ_COL_CT; ++i) { buffer[i] = ' '; }
    for (int i=0; i<SEQ_STEPS; ++i) { 
        buffer[i] = seq[i]; 
    }
    buffer[SEQ_STEPS+1] = seqOrca::ItoC(tens);
    buffer[SEQ_STEPS+2] = seqOrca::ItoC(ones); 
    buffer[SEQ_STEPS+3] = seqOrca::ItoC(pace);
    for (int i=0; i<SEQ_MAX_SCALE_SZ; ++i) { 
        buffer[SEQ_STEPS+4+i] = scale[i];
    }
    attron(COLOR_PAIR(1));
    mvwprintw(stdscr, SEQ_ROW_CT+1,0, "%s", buffer); 
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(8));
    mvaddch(SEQ_ROW_CT+1,step, buffer[step]); 
    attroff(COLOR_PAIR(8));
}


void seqTunerRow::update() {
    int tickCt = theMet->tick - measureStart;
    step = floor(tickCt * .03125 * pace); 
    int modStep = step % SEQ_STEPS;
    if (step>modStep) {
        step = modStep; 
        measureStart = theMet->tick; // wont work unless we get called >> once/tick
    }
    if (step!=lastStep) {
        int mod = 0;
        for (int i=0; i<step; ++i) {
            if (seq[i]!=' ') { 
                mod = seqOrca::CtoI(seq[i]);
            }
        }
        modTonic = tonic + mod;
    }
    lastStep = step;
}


void seqTunerRow::getHelpString(char *buffer) { 
    //               01234567890123456789
    sprintf(buffer, "tuner row           \ntonic %d   \nmodified tonic %d  \n0 %d %d %d %d\n %d %d %d %d\n", 
        tonic, modTonic, 
        seqOrca::CtoI(scale[0]),
        seqOrca::CtoI(scale[1]),
        seqOrca::CtoI(scale[2]),
        seqOrca::CtoI(scale[3]),
        seqOrca::CtoI(scale[4]),
        seqOrca::CtoI(scale[5]),
        seqOrca::CtoI(scale[6]),
        seqOrca::CtoI(scale[7])); 
}


int seqTunerRow::tune(int noteDegree) {
    if (noteDegree<1) { return modTonic; } // too low; just return tonic
    int sc[SEQ_MAX_SCALE_SZ];
    sc[0] = 0; 
    int scaleLen = 1; // tonic is unspecified but there
    for (int i=0; i<SEQ_MAX_SCALE_SZ; ++i) {
        if (scale[i]!=' ') {
            sc[scaleLen] = scale[i]; 
            ++scaleLen; 
        }
    } 
    int deg = noteDegree % scaleLen; // degree in octave
    int scale = (int)(floor(noteDegree/scaleLen)); 
    int noteInScale = sc[deg];
    int tuned = seqOrca::CtoI(noteInScale);
    return modTonic + (scale*12) + tuned;
}


float seqTunerRow::freq(int noteDegree) {
    int midi = tune(noteDegree); 
    float expn = float(midi-69) / 12.0;
    return 440.0 * pow(2.0, expn); 
}

