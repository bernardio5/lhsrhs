#pragma once

#include "seqDefs.h"
#include "../include/Voicer.h"

namespace stk {

/***************************************************/
/*! \class inMixer

    tick() calls tick() for NUM_ROWS instruments-- if they've been added and mix>0
    setVolume(which, vol) sets row mix level
    setInstrument(which, inst) sets Instrmnt* for inst->tick();
    row mix levels are summed and normalized to sum to <1
    tick outputs are multiplied by normalized mix levels.

    mix changes could create discontinuities! lerp more. 

    voicer jacks around with envelopes or whatever: not this. 
    voicer distributes noteOn/noteOff message: not.

    voicer was doing a lot of things that you'd rather it not try to do. 
    You'll want to keep pointers to all the instruments to talk to them directly.

    filter rows! we want to be able to have filters on one row that mod others
    addFilter(which, filt, targetRow)

*/
/***************************************************/


class inMixer : public Voicer
{
 public:
  inMixer() ;
  ~inMixer() {}
  bool inRange(int i);

  void setInst(int which, Instrmnt *inst) ;
  void setFilter(int which, Instrmnt *filt, int target);

  // NOP fns for interface
  void noteOn( StkFloat noteNumber, StkFloat amplitude) {} 
  void noteOff( StkFloat noteNumber, StkFloat amplitude) {}
  void noteOff(StkFloat amplitude ) {}
  void setFrequency( StkFloat frequency) {}
  void controlChange( int number, StkFloat value, int group=0 ) {}

  unsigned int channelsOut( void ) const { return lastFrame_.channels(); };
  const StkFrames& lastFrame( void ) const { return lastFrame_; };
  StkFloat lastOut( unsigned int channel = 0 ) { return lastFrame_[channel]; }
  StkFloat tick( unsigned int channel = 0 );
  StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );

 protected:
  Instrmnt *insts[SEQ_ROW_CT];
  StkFrames lastFrame_;
};


inline StkFloat inMixer::tick( unsigned int channel) {
    for (int j=0; j<lastFrame_.channels(); j++ ) lastFrame_[j] = 0.0;
    for (int i=0; i<SEQ_ROW_CT; ++i) {
        if (insts[i]!=0) {
            insts[i]->tick(); 
            for (int j=0; j<insts[i]->channelsOut(); j++ ) { 
                lastFrame_[j] += insts[i]->lastOut( j ); 
            }
        }
    }
    return lastFrame_[channel];
}


inline StkFrames& inMixer::tick( StkFrames& frames, unsigned int channel ) {
  unsigned int nChannels = lastFrame_.channels();

  StkFloat *samples = &frames[channel];
  unsigned int j, hop = frames.channels() - nChannels;
  if ( nChannels == 1 ) {
    for ( unsigned int i=0; i<frames.frames(); i++, samples += hop )
      *samples++ = tick();
  }
  else {
    for ( unsigned int i=0; i<frames.frames(); i++, samples += hop ) {
      *samples++ = tick();
      for ( j=1; j<nChannels; j++ )
        *samples++ = lastFrame_[j];
    }
  }
  return frames;
}

} // stk namespace


