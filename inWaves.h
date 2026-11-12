#pragma once

#include "../include/Instrmnt.h"
#include "../include/SineWave.h"
#include "../include/BlitSquare.h"
#include "../include/BlitSaw.h"
#include "../include/Noise.h"
#include "../include/OnePole.h"
#include "../include/ADSR.h"

#include "seqDefs.h"

namespace stk {

/***************************************************/
/*! \class inWaves
     \brief simple oscillator wrapper 

    This class puts a minimal ADSR wrapper around the
    basic wave types: sine, square, sawtooth. 
    Type is chosen at init.
    also there's a one-pole filter 
    also mayve overdrive? don't have one yet tho. 
    
    Controls
       - ADSR 0123
       - filter pole
       - filter width
       - overdrive amt ? TBD   duty cycle, for square waves? even care?  
       - mix volume
*/
/***************************************************/


class inWaves : public Instrmnt
{
 public:
  inWaves(char waveType);
  ~inWaves( );

  void setFrequency( StkFloat frequency );
  void keyOn( void );
  void keyOff( void );
  void noteOn( StkFloat frequency, StkFloat amplitude );
  void noteOff( StkFloat amplitude );

  void controlChange( int number, StkFloat value );

  StkFloat tick( unsigned int channel = 0 );
  StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );

 protected:
  ADSR      adsr_;
  // FileLoop  *loop_;
  char      type; 
  SineWave  *sin_; 
  BlitSquare *sq_; 
  BlitSaw   *saw_;
  Noise     *noise_; 
  StkFloat (*tickFn)(void);

  OnePole   filter_;
  StkFloat  baseFrequency_;
};


inline StkFloat inWaves::tick( unsigned int ) {
    switch (type) {
        case SEQ_IN_SINE:  lastFrame_[0] = filter_.tick( sin_->tick() ); break;
        case SEQ_IN_SQUARE:lastFrame_[0] = filter_.tick( sq_->tick() );   break;
        case SEQ_IN_SAW:   lastFrame_[0] = filter_.tick( saw_->tick() ); break;
        case SEQ_IN_NOISE: lastFrame_[0] = filter_.tick( noise_->tick()); break;
    }
    lastFrame_[0] *= adsr_.tick();
    return lastFrame_[0];
}


inline StkFrames& inWaves::tick( StkFrames& frames, unsigned int channel ) {
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


