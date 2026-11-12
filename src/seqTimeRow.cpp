
#include "seqTimeRow.h"

void seqTimeRow::init(seqMet *m) {
    theMet = m; 
    // more normal for rows
    timer.initC('0'); // how long till next keyframes
    lerpTime = 0; // how long the lerps will be
    hundreds = 1;
    tens = 2;
    ones = 0;
    bpm.init(120.0/250.0);
    r.init(0.0); // reverb
    d.init(0.0); // delay
    lastStep = SEQ_STEPS -1; // first one flips us over 
}


void seqTimeRow::setBPM() {
    float newBPM = (float)((hundreds*100) + (tens*10) + ones);
    if (newBPM>250.0) {
        newBPM = 250.0; 
        hundreds = 2;
        tens = 5;
        ones = 0;
    }
    if (newBPM<15.0) {
        newBPM = 15.0; 
        hundreds = 0;
        tens = 1;
        ones = 5;
    }
    bpm.editF(newBPM / 250.0); // I think? ready to be keyed normally.
}


void seqTimeRow::edit(int col, char ch) {
    switch (col) { 
      case SEQ_STEPS + 0: timer.setC(ch); break;
      case SEQ_STEPS + 1: lerpTime = seqOrca::CtoI(ch); break;
      case SEQ_STEPS + 4: hundreds = seqOrca::CtoI(ch); setBPM(); break; 
      case SEQ_STEPS + 5: tens = seqOrca::CtoI(ch); setBPM(); break; 
      case SEQ_STEPS + 6: ones = seqOrca::CtoI(ch); setBPM(); break; 
      case SEQ_STEPS +10: r.editC(ch); break;
      case SEQ_STEPS +11: d.editC(ch); break;
    }
}


void seqTimeRow::draw(WINDOW *stdscr) {
    char buffer[SEQ_COL_CT];
    for (int i=0; i<SEQ_COL_CT; ++i) { buffer[i] = ' '; }
    buffer[SEQ_STEPS+0] = timer.c;
    buffer[SEQ_STEPS+1] = seqOrca::ItoC(lerpTime);
    buffer[SEQ_STEPS+4] = seqOrca::ItoC(hundreds);  // get left-side BPM from hund, tens, ones
    buffer[SEQ_STEPS+5] = seqOrca::ItoC(tens);
    buffer[SEQ_STEPS+6] = seqOrca::ItoC(ones);
    buffer[SEQ_STEPS+10] = r.getEditC();
    buffer[SEQ_STEPS+11] = d.getEditC();
    float bpmv = (bpm.val())*250.0;  // get rt-side BPM from bpm keyable, not hundreds.display etc
    int h = floor(bpmv/100.0);
    int t = floor((bpmv-(h*100.0))/10.0);
    int o = floor(bpmv-(h*100.0)-(t*10.0));
    buffer[SEQ_RT_START+lastStep] = '+';
    buffer[SEQ_RT_START+SEQ_STEPS+4] = seqOrca::ItoC(h);
    buffer[SEQ_RT_START+SEQ_STEPS+5] = seqOrca::ItoC(t);
    buffer[SEQ_RT_START+SEQ_STEPS+6] = seqOrca::ItoC(o);
    buffer[SEQ_RT_START+SEQ_STEPS+10] = r.getCurrentC();
    buffer[SEQ_RT_START+SEQ_STEPS+11] = d.getCurrentC();
    attron(COLOR_PAIR(1));
    mvwprintw(stdscr, 0,0, "%s", buffer); 
    attroff(COLOR_PAIR(1));
}


void seqTimeRow::update() {
    bpm.update(theMet->lastTime); 
    r.update(theMet->lastTime); 
    d.update(theMet->lastTime); 
    theMet->setBPM(bpm.val()*250.0);
    int step = (theMet->tick >> 2) % SEQ_STEPS;  // tick/4 is steps  
    if (lastStep>step) { 
      if (timer.dec()) { 
        // if timer is going to 0, 
        float lerpT = ((float)lerpTime) * (theMet->tickFreq * SEQ_STEPS);
        bpm.key(lerpT, theMet->lastTime); // should work normally? eeehh?
        r.key(lerpT, theMet->lastTime); 
        d.key(lerpT, theMet->lastTime);
      }
    }
    lastStep = step; 
}


void seqTimeRow::getHelpString(char *buffer) { 
    //               01234567890123456789
    sprintf(buffer, "time row             \ntick %d  step %d      \nbpm: %f          \n", 
        theMet->tick, lastStep, (bpm.val())*250.0); 
}

