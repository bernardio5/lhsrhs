#pragma once
#include "../include/Instrmnt.h"
#include "../include/ADSR.h"
#include "../include/FileLoop.h"
#include "../include/OnePole.h"
#include "../include/BiQuad.h"
#include "../include/Noise.h"

namespace stk {

/***************************************************/
/*! \class inSynDrum
    \brief wavetable/noise percussion

    Combines noise and ringing, with different ADSR's
    ADSR waits for note off! 

    Control Change Numbers: 
      Noise ADSR: A=0, D=1, max=2,  SR===0
      Noise Filter Pole: 2
      3===x
      Ringtone ADSR: A=4, D=5, max=6, SR===0
      ring waveform can be sin or square; crossfade of that is 

    Two generators, one noise & one tone
    Each has its own triangular envelope, specified by
      max val, time to max, time to get back to 0
      these are linear? why not. 
    Note-off messages are ignored?



  don't need sustain/release, 


    by Perry R. Cook and Gary P. Scavone, 1995--2023.
*/
/***************************************************/

class inSynDrum : public Instrmnt
{
 public:
  //! Class constructor.
  /*!
    An StkError will be thrown if the rawwave path is incorrectly set.
  */
  inSynDrum( void );

  //! Class destructor.
  ~inSynDrum( void );

  //! Set instrument parameters for a particular frequency.
  void setFrequency( StkFloat frequency );

  //! Start envelope toward "on" target.
  void keyOn( void );

  //! Start envelope toward "off" target.
  void keyOff( void );

  //! Start a note with the given frequency and amplitude.
  void noteOn( StkFloat frequency, StkFloat amplitude );

  //! Stop a note with the given amplitude (speed of decay).
  void noteOff( StkFloat amplitude );

  //! Perform the control change specified by \e number and \e value (0.0 - 128.0).
  void controlChange( int number, StkFloat value );

  //! Compute and return one output sample.
  StkFloat tick( unsigned int channel = 0 );

  //! Fill a channel of the StkFrames object with computed outputs.
  /*!
    The \c channel argument must be less than the number of
    channels in the StkFrames argument (the first channel is specified
    by 0).  However, range checking is only performed if _STK_DEBUG_
    is defined during compilation, in which case an out-of-range value
    will trigger an StkError exception.
  */
  StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );

 protected:

  ADSR      adsr_; 
  FileLoop *loop_;
  OnePole   filter_;
  BiQuad    biquad_;
  Noise     noise_;
  StkFloat  baseFrequency_;
  StkFloat  loopGain_;

};

inline StkFloat inSynDrum :: tick( unsigned int )
{
  lastFrame_[0] = loopGain_ * loop_->tick();
  biquad_.tick( noise_.tick() );
  lastFrame_[0] += (1.0 - loopGain_) * biquad_.lastOut();
  lastFrame_[0] = filter_.tick( lastFrame_[0] );
  lastFrame_[0] *= adsr_.tick();
  return lastFrame_[0];
}

inline StkFrames& inSynDrum :: tick( StkFrames& frames, unsigned int channel )
{
  unsigned int nChannels = lastFrame_.channels();
#if defined(_STK_DEBUG_)
  if ( channel > frames.channels() - nChannels ) {
    oStream_ << "inSynDrum::tick(): channel and StkFrames arguments are incompatible!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }
#endif

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

