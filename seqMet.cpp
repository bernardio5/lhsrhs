#include "seqMet.h"


void seqMet::init() {
    sampRate = 44100.0; 
    tick = 0;
    remainingTickSamples = 0; 
    floatTick = 0.0;
    setBPM(120.0);  
    lastTime = -1.0;
}


void seqMet::setBPM(float bpm) { 
    if (bpm<15.0f) { tickFreq = 4.0; } 
    else { 
      // ticks/sec = beats/min * ticks/beat * min/sec 
      //           = 120       * 16          *   1/60   
      tickFreq = bpm * 0.266666666666667;   // 4/60=.0667
    }
    sampPerTick = sampRate / tickFreq; 
}


// t=time, in seconds, since program start
void seqMet::timeUpdate(double t) {
    if (lastTime<0.0) {
      lastTime = t;
      if (remainingTickSamples<1) { remainingTickSamples = sampPerTick; }
    } else {
      floatTick += ((t-lastTime) * tickFreq);
      double fractpart, intpart; 
      fractpart = modf(floatTick , &intpart);
      tick = (int)intpart;
      lastTime = t; 
      remainingTickSamples = floor((1.0-fractpart) * sampPerTick);  
      if (remainingTickSamples<1) { remainingTickSamples = sampPerTick; }
    }
}



void seqMet::sampleUpdate(int numSamples) {
    double dt = numSamples / sampRate; 
    floatTick += (dt * tickFreq);
    double fractpart, intpart; 
    fractpart = modf(floatTick , &intpart);
    tick = (int)intpart;
    lastTime += dt; 
    remainingTickSamples = floor((1.0-fractpart) * sampPerTick);  
    if (remainingTickSamples<1) { remainingTickSamples = sampPerTick; }
}

