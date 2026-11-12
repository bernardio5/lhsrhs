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

#include "inWaves.h"
#include <cmath>

namespace stk {

inWaves :: inWaves( char wavetype ) {
  type = wavetype;
  switch (wavetype) {
    case SEQ_IN_SINE:     sin_ = new SineWave();    break; 
    case SEQ_IN_SQUARE:   sq_ = new BlitSquare();   break; 
    case SEQ_IN_SAW:      saw_ = new BlitSaw();     break;
    case SEQ_IN_NOISE:    noise_ = new Noise();     break;
  }
//  loop_ = new FileLoop( (Stk::rawwavePath() + "impuls10.raw").c_str(), true );
  filter_.setPole( 0.5 );
  baseFrequency_ = 440.0;
  setFrequency( baseFrequency_ );
  adsr_.setAllTimes( 0.05, 0.05, 0.9, 0.05 ); // 22v2ixxm  
}

inWaves :: ~inWaves() { }

void inWaves :: setFrequency( StkFloat frequency ) {
  switch (type) {
    case SEQ_IN_SINE:     sin_->setFrequency(frequency);  break; 
    case SEQ_IN_SQUARE:   sq_->setFrequency(frequency);  break; 
    case SEQ_IN_SAW:      saw_->setFrequency(frequency);  break; 
    case SEQ_IN_NOISE:    break; // set filter frequency? 
  }  
}

void inWaves::keyOn( void ) { adsr_.keyOn(); }

void inWaves::keyOff( void ) { adsr_.keyOff(); }

void inWaves::noteOn( StkFloat frequency, StkFloat amplitude ) {
  keyOn();
  setFrequency( frequency );
  filter_.setGain( amplitude ); 
}

void inWaves :: noteOff( StkFloat amplitude ) {
  keyOff();
}

void inWaves :: controlChange( int number, StkFloat value ) {
  switch (number) { 
    case 0: adsr_.setAttackTime( value ); break; 
    case 1: adsr_.setDecayTime( value ); break; 
    case 2: adsr_.setSustainLevel( value ); break; 
    case 3: adsr_.setReleaseTime( value ); break; 
    case 4: filter_.setPole(value); break;
    case 5: break; 
    case 6: break; 
  }
}

} // stk namespace
