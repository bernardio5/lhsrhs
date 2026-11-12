#include "seqRow.h"


void seqRow::init(int rw, seqMet *m, seqTunerRow *t, inMixer *mx,
      char whichInst, char *initSeq, int lv) {
    myRow = rw;
    theMet = m;
    myInst.init(myRow, whichInst, t, mx);
    // message init below
    isLive = lv;
    timer.initF(0.0); // how long till next keyframes
    lerpTime = '0'; // how long the lerps will be
    editInst = whichInst;
    for (int i=0; i<SEQ_STEPS; ++i) { 
      editSeq[i] = initSeq[i];
      playingSeq[i] = initSeq[i];
    }
    pace = 8;
    editPace = 8;  // tempo (BPM) is global. tempo for this row is global*(pace/8)
    step = -1; 
    lastStep = -1; 
    measureStart = -2;
    lastEvtT = -99.0;
    
    // get the instrument's defaults and use them to set if controls. 
    std::string defs = myInst.getDefaults(); 
    float keyVals[SEQ_CTRL_CT];
    for (int i=0; i<SEQ_CTRL_CT; ++i) {
      keyVals[i] = seqOrca::CtoF(defs[i]);
      keys[i].init(keyVals[i]);
    }
    sprintf(debugMsg, "---");
}


void seqRow::edit(int col, char ch) {
    if (col<SEQ_STEPS) {
        editSeq[col] = ch; 
        return;
    }
    if (col<SEQ_RT_START) {
      switch (col) {
        case 16: timer.setC(ch); break;
        case 17: lerpTime = ch; break;
        case 18: editInst = ch; break;
        case 19: 
          editPace = seqOrca::CtoI(ch); 
          if (editPace<1) editPace = 1; 
          break;
        default: keys[col-(SEQ_STEPS+4)].editC(ch); break;
      }
    }
}



// for all of these: cut copy paste work on either controls or seq
// they are (copied to)/(pasted from) a single set of char arrays
// timer, lerptimer, instrument and pace are not affected. 

void seqRow::cut(int col, char *cutSeq, char *cutSettings) {
  if (col<SEQ_STEPS) {
    for (int i=0; i<SEQ_STEPS; ++i) { 
      cutSeq[i] = editSeq[i]; 
      editSeq[i] = ' ';
    }
  } else if (col>(SEQ_STEPS+3)) { 
    std::string defs = myInst.getDefaults(); 
    for (int i=0; i<SEQ_CTRL_CT; ++i) { 
      cutSettings[i] = seqOrca::FtoC(keys[i].val());
      keys[i].editC(defs[i]);
    }
  }
}

void seqRow::copy(int col, char *cutSeq, char *cutSettings) {
  if (col<SEQ_STEPS) {
    for (int i=0; i<SEQ_STEPS; ++i) { 
      cutSeq[i] = editSeq[i]; 
    }
  } else if (col>(SEQ_STEPS+3)) { 
    for (int i=0; i<SEQ_CTRL_CT; ++i) { 
      cutSettings[i] = seqOrca::FtoC(keys[i].val());
    }
  }
}

void seqRow::paste(int col, char *cutSeq, char *cutSettings) {
  if (col<SEQ_STEPS) {
    for (int i=0; i<SEQ_STEPS; ++i) { 
      editSeq[i] = cutSeq[i];
    }
  } else if (col>(SEQ_STEPS+3)) { 
    for (int i=0; i<SEQ_CTRL_CT; ++i) { 
      keys[i].editC(cutSettings[i]);
    }
  }
}

void seqRow::increment(int col) {
  if (col<SEQ_STEPS) {
    int i = seqOrca::CtoI(editSeq[col]);
    if (i<35) { 
      editSeq[col] = seqOrca::ItoC(i+1);
    }
  } else if (col>(SEQ_STEPS+3)) { 
    int ct = col-(SEQ_STEPS+4);
    int i = seqOrca::CtoI(keys[ct].getEditC());
    if (i<35) { 
      keys[ct].editC(seqOrca::ItoC(i+1));
    }
  } else {
    switch (col) { 
    case SEQ_STEPS+0: timer.inc(); break;
    case SEQ_STEPS+1: 
      int i = seqOrca::CtoI(lerpTime); 
      if (i<35) { lerpTime = seqOrca::ItoC(i+1); } 
      break;
    // can't inc instruments?
    // case SEQ_STEPS+3: ++editPace; break; // inc pace? INadvisable
    }
  }
}

void seqRow::decrement(int col) {
  if (col<SEQ_STEPS) {
    int i = seqOrca::CtoI(editSeq[col]);
    if (i>0) { 
      editSeq[col] = seqOrca::ItoC(i-1);
    }
  } else if (col>(SEQ_STEPS+3)) { 
    int ct = col-(SEQ_STEPS+4);
    int i = seqOrca::CtoI(keys[ct].getEditC());
    if (i>0) { 
      keys[ct].editC(seqOrca::ItoC(i-1));
    }
  } else {
    switch (col) { 
    case SEQ_STEPS+0: timer.dec(); break;
    case SEQ_STEPS+1: 
      int i = seqOrca::CtoI(lerpTime); 
      if (i>0) { lerpTime = seqOrca::ItoC(i-1); } 
      break;
    // can't inc instruments?
    // case SEQ_STEPS+3: ++editPace; break; // inc pace? INadvisable
    }
  }
}




void seqRow::update() {
    bool doCopy = false; // whether to copy LHS to RHS
    int tick = theMet->tick - measureStart; 
    step = (tick * pace)>>5; 
    //if (isLive) {
    //  doCopy = true; // always copy over if live
    //}
    if (step>15) { // new measure 
      step = step % SEQ_STEPS; 
      measureStart = theMet->tick; // formal: new measure now.  
      if (timer.dec()) { 
        doCopy = true;         // if timer is going to 0, copy
      }
    }
    if (doCopy) { // (new measure AND timer going off) OR live
      for (int i=0; i<SEQ_STEPS; ++i) {
        playingSeq[i] = editSeq[i]; 
      }
      myInst.editType(editInst); // if unchanged, does nothing
      pace = editPace;
      float lerpSteps = (float)(seqOrca::CtoI(lerpTime));
      //             #measures * steps/mes * ticks/step      / ticks/sec
      float lerpT = (lerpSteps * SEQ_STEPS * 0.03125 * pace) / theMet->tickFreq;
      for (int i=0; i<SEQ_CTRL_CT; ++i) { 
        keys[i].key(lerpT, theMet->lastTime); 
      }
    }
    if (step!=lastStep) { // new step
      for (int i=0; i<SEQ_CTRL_CT; ++i) { keys[i].update(theMet->lastTime); }
      char evt = playingSeq[step];
      if (evt!=' ') { // there is a seq event
        myInst.trigger(evt, keys); // use keys and inst to set sound params
          if (myRow==0) { 
            sprintf(debugMsg, "msg evt:'%c' step:%d, tk:%d", evt, step, theMet->tick);
          }
          lastEvtT = theMet->lastTime; 
      }
    }
    lastStep = step;
}



void seqRow::draw(WINDOW *win, int row) {
    char buffer[SEQ_COL_CT];
    for (int i=0; i<SEQ_COL_CT; ++i) { buffer[i] = ' '; }
    for (int i=0; i<SEQ_STEPS; ++i) { 
        buffer[i] = editSeq[i]; 
        buffer[SEQ_RT_START+i] = playingSeq[i];
    }
    buffer[SEQ_STEPS+0] = timer.c;
    buffer[SEQ_STEPS+1] = lerpTime;
    buffer[SEQ_STEPS+2] = editInst;
    buffer[SEQ_STEPS+3] = seqOrca::ItoC(editPace);
    buffer[SEQ_RT_START+SEQ_STEPS+2]= myInst.myType;
    buffer[SEQ_RT_START+SEQ_STEPS+3]= seqOrca::ItoC(pace);
    for (int i=0; i<SEQ_CTRL_CT; ++i) { 
      buffer[SEQ_STEPS+4+i] = keys[i].getEditC();
      buffer[SEQ_RT_START+SEQ_STEPS+4+i] = keys[i].getCurrentC();  
    }
    // neal, this is horrid. but it sets colors in nice 4x cols
    char cols[] = "53531675353167";
    for (int i=0; i<SEQ_COL_CT; ++i) {
      int sel = floor(i/4); 
      int col = (int)cols[sel] - (int)('0');
      attron(COLOR_PAIR(col));
      mvaddch(row,i, buffer[i]); 
      attroff(COLOR_PAIR(col));
    }  // design decisions should live in squalor maybe
    int stepPl = SEQ_RT_START + step; 
    attron(COLOR_PAIR(8));
    mvaddch(row,stepPl, buffer[stepPl]); 
    attroff(COLOR_PAIR(8));
    if (myRow==0) { 
      mvwprintw(stdscr, 17,0, "%s", debugMsg); 
    }
}



void seqRow::getHelpString(char *buffer) { 
  //                 01234567890123456789
    std::string hs = myInst.getInfo(); 
    sprintf(buffer, "sequencer row       \nst %d pc %d ms %d \n 0 %4.2f 1 %4.2f\n2 %4.2f 3 %4.2f\n4 %4.2f 5 %4.2f\n6 %4.2f 7 %4.2f\n%s\n",
          step, pace, measureStart,
          keys[0].val(),keys[1].val(),keys[2].val(),
          keys[3].val(),keys[4].val(),keys[5].val(),
          keys[6].val(),keys[7].val(),
          hs.c_str()); 
}
