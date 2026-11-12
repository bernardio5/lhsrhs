#pragma once

//  #include "seqDefs.h"
#include <cmath>

// metronome   tracks ticks and BPM
// we are assuming 4/4 time. but the sequencers all do 16 divisions/measure!
// Volcas and others specify tempo in BPM, but fudge it so that 120 BPM makes 
// each measure last 2 sec, not 8

// so: ticks! Ticks are beats/4. 

// to do ticks correctly, at last, need to integrate over t
// tick(t1) = .5*a(t0)*(t1-t0)*(t1-t0) + v(t0)*(t1-t0) + t0

// implement after we know what STK needs


// okay: seqMet has to have an integral counter that absolutely synchronizes all rows. 
// rows MAY NOT compute their own ticks/steps: they lose time each measure, 
// and it's not subtle. so: measures! 4 beats per measure. steps! 4 steps per beat. 

// ticks! a measure is always 16 steps, and each step can have 1-36 ticks, as set by rows' slowdown. 
// the default is 4. set it to 2, it goes twice as fast. 1, 4 times, 8:half 16:quarter 32:eighth

// theMet's tick counter is computed from BPM only. 



class seqMet {

public:
    seqMet() {}
    void init(); 
    void setBPM(float bpm);

    // to be called at control rates: 1/64 audio rate: thousands of times a second
    void timeUpdate(double t); 
    // called after computing up to 64 samples after last time update
    void sampleUpdate(int numSamples);
  
    int tick, remainingTickSamples; 
    double floatTick, tickFreq, lastTime, sampRate, sampPerTick; 
};

// love me love my egyptian wordplay

