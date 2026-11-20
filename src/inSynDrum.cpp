/***************************************************/
/*! \class inSynDrum
    \brief STK wavetable/noise instrument.

    This class combines a looped wave, a
    noise source, a biquad resonance filter,
    a one-pole filter, and an ADSR envelope
    to create some interesting sounds.

    Control Change Numbers: 
       - Filter Pole Position = 2
       - Noise/Pitched Cross-Fade = 4
       - Envelope Rate = 11
       - Gain = 128

    by Perry R. Cook and Gary P. Scavone, 1995--2023.
*/
/***************************************************/

#include "inSynDrum.h"
// #include "SKINImsg.h"

namespace stk {

inSynDrum :: inSynDrum( void )
{
  // Concatenate the STK rawwave path to the rawwave file
  loop_ = new FileLoop( (Stk::rawwavePath() + "impuls10.raw").c_str(), true );

  filter_.setPole( 0.5 );
  baseFrequency_ = 440.0;
  setFrequency( baseFrequency_ );
  loopGain_ = 0.5;
}  

inSynDrum :: ~inSynDrum( void )
{
  delete loop_;
}

void inSynDrum :: keyOn( void )
{
  adsr_.keyOn();
}

void inSynDrum :: keyOff( void )
{
  adsr_.keyOff();
}

void inSynDrum :: noteOn( StkFloat frequency, StkFloat amplitude )
{
  this->keyOn();
  this->setFrequency( frequency );
  filter_.setGain( amplitude ); 
}
void inSynDrum :: noteOff( StkFloat amplitude )
{
  this->keyOff();
}

void inSynDrum :: setFrequency( StkFloat frequency )
{
#if defined(_STK_DEBUG_)
  if ( frequency <= 0.0 ) {
    oStream_ << "inSynDrum::setFrequency: argument is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }
#endif

  biquad_.setResonance( frequency, 0.98, true );
  loop_->setFrequency( frequency );
}

void inSynDrum :: controlChange( int number, StkFloat value )
{
  // avoid 0!
  switch (number) {
    case 0: adsr_.setAttackTime( value + 0.01 ); break; 
    case 1: adsr_.setDecayTime( (0.1 * value) + 0.01 ); break; 
    case 2: adsr_.setSustainLevel( value ); break; 
    case 3: adsr_.setReleaseTime( (3.0 * value) + 0.01 ); break; 
    case 4: filter_.setPole( 0.99 * (1.0 - (value * 2.0)) );break; 
    case 5: loopGain_ = value; break; 
    default: break; 
  }
}

} // stk namespace
