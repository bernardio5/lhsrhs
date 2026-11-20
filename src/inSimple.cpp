/***************************************************/
/*! \class inSimple2
    \brief STK wavetable/noise instrument.

*/
/***************************************************/

#include "inSimple2.h"



namespace stk {

inSimple2 :: inSimple2( void )
{
  // Concatenate the STK rawwave path to the rawwave file
  loop_ = new FileLoop( (Stk::rawwavePath() + "impuls10.raw").c_str(), true );

  filter_.setPole( 0.5 );
  baseFrequency_ = 440.0;
  setFrequency( baseFrequency_ );
  loopGain_ = 0.5;
}  

inSimple2 :: ~inSimple2( void )
{
  delete loop_;
}

void inSimple2 :: keyOn( void )
{
  adsr_.keyOn();
}

void inSimple2 :: keyOff( void )
{
  adsr_.keyOff();
}

void inSimple2 :: noteOn( StkFloat frequency, StkFloat amplitude )
{
  this->keyOn();
  this->setFrequency( frequency );
  filter_.setGain( amplitude ); 
}
void inSimple2 :: noteOff( StkFloat amplitude )
{
  this->keyOff();
}

void inSimple2 :: setFrequency( StkFloat frequency )
{
#if defined(_STK_DEBUG_)
  if ( frequency <= 0.0 ) {
    oStream_ << "inSimple2::setFrequency: argument is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }
#endif

  biquad_.setResonance( frequency, 0.98, true );
  loop_->setFrequency( frequency );
}

void inSimple2 :: controlChange( int number, StkFloat value )
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
