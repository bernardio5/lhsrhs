#include "seqEditor.h"

seqMet seqEditor::theMet;
seqTimeRow seqEditor::timeRow; 
seqRow seqEditor::seqRows[SEQ_ROW_CT];
seqTunerRow seqEditor::tunerRow; 

// message.time is "time in future" DELTA_CONTROL_TICKS = ??

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio samples.
int seqEditor::ticker( 
    void *outputBuffer, void *inputBuffer, 
    unsigned int nBufferFrames, double streamTime, 
    RtAudioStreamStatus status, void *dataPointer ) {

    TickData *data = (TickData *) dataPointer;
    StkFloat *samples = (StkFloat *) outputBuffer;
    int samplesToDo, nTicks = (int) nBufferFrames;
    bool tendToRows; 

    theMet.timeUpdate(streamTime); // get next tick!
    timeRow.update(); // .011 sec updates are fine
    tunerRow.update();
    for (int i=0; i<SEQ_ROW_CT; ++i) { 
        seqRows[i].update(); 
    }

    while ( nTicks > 0 ) { // ->done is set true by "exit" message (?)
        samplesToDo = theMet.remainingTickSamples;
        tendToRows = true;     
        if (nTicks<samplesToDo) { 
            samplesToDo = nTicks; // not enough samples left for this buffer
            tendToRows = false;   // to get to the next tick boundary
        }
        for ( int i=0; i<samplesToDo; i++ ) {
            *samples++ = data->mixer.tick();
            nTicks--;
        }

        theMet.sampleUpdate(samplesToDo);
        if (tendToRows) { 
            for (int i=0; i<SEQ_ROW_CT; ++i) { 
                seqRows[i].update(); 
            }
        }
    }
    data->streamTime = streamTime; 
    data->counter = 0; 
    return 0;
}


void seqEditor::init() {
    caretX = 2; 
    caretY = 0; 
    keepOn = true;
    theMet.init(); 
    timeRow.init(&theMet);
    tunerRow.init(&theMet);

    char ini[] = "0123456789abcdef";
    for (int i=0; i<SEQ_STEPS; ++i) { 
        cutSeq[i] = ini[i]; 
    }
    for (int i=0; i<SEQ_CTRL_CT; ++i) { 
        cutSettings[i] = ini[i]; 
    }

    data.counter = 0; 
    // Set the global sample rate and rawwave path before creating class instances.
    Stk::setSampleRate( 44100.0 );
    Stk::setRawwavePath( "data/" );
    // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
    parameters.deviceId = dac.getDefaultOutputDevice();
    parameters.nChannels = 1;
    RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
    unsigned int bufferFrames = RT_BUFFER_SIZE;
    if ( dac.openStream( &parameters, NULL, format, 
        (unsigned int)Stk::sampleRate(), 
        &bufferFrames, &ticker, (void *)&data ) ) {
        abort(1, false);
        return;
    }

    char initSeq[] = "4 1 1 1 4 1 111 "; // initial seq content for row 0
    for (int i=0; i<SEQ_ROW_CT; ++i) {       
        try {
            char inst = seqOrca::ItoC(i+1);
            seqRows[i].init(i, &theMet, &tunerRow, &(data.mixer), inst, initSeq, 0);
        } catch ( StkError & ) {
            abort(3, false);
            return;
        }
        sprintf(initSeq, "                "); // non-0 rows initially blank
    }

    if ( dac.startStream() ) {
        abort(3, false);
        return;
    }

    //logfile = fopen("log.txt", "w");
    //fprintf(logfile, "ready\n"); 
}

    
void seqEditor::abort(int source, bool streamToo) { 
    keepOn = false;
    std::string er = dac.getErrorText();
    if (streamToo) { dac.closeStream(); }
}



// seqMet updates called from inside SDK. 
// this is screen-rate, maybe 1000 times slower. 
// just redraw UI and deal with keystrokes. 
void seqEditor::draw(WINDOW *win) {
    timeRow.draw(win);
    for (int i=0; i<SEQ_ROW_CT; ++i) {
        seqRows[i].draw(win, i+1);
    }
    tunerRow.draw(win); 
    switch (caretY) {
      case  0: timeRow.getHelpString(helpString); break; 
      case  SEQ_ROW_CT+1: tunerRow.getHelpString(helpString); break; 
      default:
        if (caretY<SEQ_ROW_CT+1) {
            seqRows[caretY-1].getHelpString(helpString); 
        }
        break;
    }
    int hsx=58; 
    int hsy=2; 
    attron(COLOR_PAIR(2));
    for (int i=0; i<strlen(helpString); ++i) { 
        if (helpString[i]=='\n') { hsx=58; ++hsy; }
        else { mvaddch(hsy, hsx, helpString[i]); ++hsx; }
    }
    attroff(COLOR_PAIR(2));
    // mvwprintw(stdscr, 15,0, "%s", editString);  
}


void seqEditor::directEdit(int row, int col, char c) {
    // sprintf(editString, "editRow r %d c %d ch %c", row, col, c);
    switch (row) {
      case  0:                
        timeRow.edit(col, c); 
        break; 
      case  SEQ_ROW_CT+1:     
        tunerRow.edit(col, c); 
        break; 
      default:                
        if (row<SEQ_ROW_CT+1) {
          seqRows[row-1].edit(col, c);     
        }
        break; 
    }    
}

// if in seq, copy paste that. if in settings, those. 
// otherwise, do nothing b/c other areas are not compatible. 
void seqEditor::cutRow(int row, int col) { 
    if ((row>0) && (row<=SEQ_ROW_CT)) {
        seqRows[row-1].cut(col, cutSeq, cutSettings);
    }
}

void seqEditor::copyRow(int row, int col) { 
        if ((row>0) && (row<=SEQ_ROW_CT)) {
        seqRows[row-1].copy(col, cutSeq, cutSettings);
    }
}

void seqEditor::pasteRow(int row, int col) { 
        if ((row>0) && (row<=SEQ_ROW_CT)) {
        seqRows[row-1].paste(col, cutSeq, cutSettings);
    }
}

void seqEditor::undoRow() { }


void seqEditor::increment(int row, int col) {
    if ((row>0) && (row<=SEQ_ROW_CT)) {
        seqRows[row-1].increment(col);
    }
}

void seqEditor::decrement(int row, int col) { 
    if ((row>0) && (row<=SEQ_ROW_CT)) {
        seqRows[row-1].decrement(col);
    }
}


void seqEditor::keyPressed (int key) {
    char c = (char)(key%256); 
    sprintf(editString, "key in %d ch %c tick %d", key, c, theMet.tick);
    switch (key) {
    case KEY_LEFT:      if (caretX>0)               { --caretX; } break; 
    case KEY_RIGHT:     if (caretX<SEQ_RT_START-1)  { ++caretX; } break; 
    case KEY_UP:        if (caretY>0)               { --caretY; } break; 
    case KEY_DOWN:      if (caretY<SEQ_ROW_CT+1)    { ++caretY; } break; 
    case KEY_BACKSPACE: 
        if (caretX>0) { --caretX; } 
        directEdit(caretY, caretX, ' '); 
        break;
    case KEY_HOME:      caretX = 0;             break;
    case KEY_END:       caretX = 16;            break;
    case KEY_NPAGE:     caretY = SEQ_ROW_CT;    break;
    case KEY_PPAGE:     caretY = 0;             break;
        // control characters
    case 23: if (caretY>0) { --caretY; }            break; // ctrl-w up  ^W home?
    case  1: if (caretX>0) { --caretX; }            break; // ctrl-a left
    case 19: if (caretY<SEQ_ROW_CT+1) { ++caretY; } break; // ctrl-s down
    case  4: if (caretX<SEQ_RT_START-1) { ++caretX;} break;// ctrl-d right
    case 24: cutRow(caretY, caretX);            break; // ctrl-X cut
    case  3: copyRow(caretY, caretX);           break; // ctrl-C copy
    case 22: pasteRow(caretY, caretX);          break; // ctrl-V paste

    //case  6: fourFloor();               break; // ^f 4-onna-flr  set inst to drum, oom-ch oom-ch
    case  7: increment(caretY, 16);     break; // ^g increment the timer ("go!")
    case 10: directEdit(caretY, (SEQ_STEPS+4+7), '0'); break; // ^m mute: set ctrl7=0 
    // case 11: break; // ^k clear-- seq and set ctrls to inst defaults
    // ? merge-- sync all rows to measure start
    case 17: keepOn = false; break; // ctrl-q quit!
    case 26: undoRow();  break; // ctrl-Z undo -- sure, baby. sure. 
    case 45: decrement(caretY, (SEQ_STEPS+1)); break;     // -/=(+) lerp -/+  
    case 61: increment(caretY, (SEQ_STEPS+1)); break;
    case 91: decrement(caretY, (SEQ_STEPS+4+7)); break;    // [/] vol -/+  
    case 93: increment(caretY, (SEQ_STEPS+4+7)); break;
    default:
        if (((c>='0')&&(c<='9')) || 
            ((c>='a')&&(c<='z')) || 
    		(c == ' ') || 
            (c == '.')) {
            directEdit(caretY, caretX, c);
            if (caretX<SEQ_RT_START-1)  { ++caretX; } 
    	}
    }
}


