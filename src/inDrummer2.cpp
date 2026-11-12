/***************************************************/
/*! \class inDrummer2
    \brief STK drum sample player class.

    This class implements a drum sampling
    synthesizer using FileWvIn objects and one-pole
    filters.  The drum rawwave files are sampled
    at 22050 Hz, but will be appropriately
    interpolated for other sample rates.  You can
    specify the maximum polyphony (maximum number
    of simultaneous voices) via a #define in the
    Drummer.h.

    by Perry R. Cook and Gary P. Scavone, 1995--2023.
*/
/***************************************************/

#include "inDrummer2.h"
#include <cmath>
#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


namespace stk {

				  
char waveNames[DRUM_NUMWAVES][28] =
  { 
    "bassdrum.raw",
    "snardrum.raw",
    "tomlowdr.raw",

    "tommiddr.raw",
    "tomhidrm.raw",
    "hihatcym.raw",
    
    "ridecymb.raw",
    "crashcym.raw", 
    "cowbell1.raw", 
    
    "tambourn.raw", 
    "conga-open-1.wav",
    "cymbal-hihat-foot-1.wav"
  };

inDrummer2 :: inDrummer2( void ) : Instrmnt()
{
  // This counts the number of sounding voices.
  nSounding_ = 0;
  soundOrder_ = std::vector<int> (DRUM_POLYPHONY, -1);
  soundNumber_ = std::vector<int> (DRUM_POLYPHONY, -1);
}

inDrummer2 :: ~inDrummer2( void )
{
}

void inDrummer2 :: noteOn( StkFloat instrument, StkFloat amplitude )
{


  int noteNumber = (int)(floor(instrument)) % DRUM_NUMWAVES;

  // If we already have a wave of this note number loaded, just reset
  // it.  Otherwise, look first for an unused wave or preempt the
  // oldest if already at maximum polyphony.
  int iWave;
  for ( iWave=0; iWave<DRUM_POLYPHONY; iWave++ ) {
    if ( soundNumber_[iWave] == noteNumber ) {
      if ( waves_[iWave].isFinished() ) {
        soundOrder_[iWave] = nSounding_;
        nSounding_++;
      }
      waves_[iWave].reset();
      filters_[iWave].setPole( 0.999 - (amplitude * 0.6) );
      filters_[iWave].setGain( amplitude );
      break;
    }
  }

  if ( iWave == DRUM_POLYPHONY ) { // This note number is not currently loaded.
    if ( nSounding_ < DRUM_POLYPHONY ) {
      for ( iWave=0; iWave<DRUM_POLYPHONY; iWave++ )
        if ( soundOrder_[iWave] < 0 ) break;
      nSounding_ += 1;
    }
    else { // interrupt oldest voice
      for ( iWave=0; iWave<DRUM_POLYPHONY; iWave++ )
        if ( soundOrder_[iWave] == 0 ) break;
      // Re-order the list.
      for ( int j=0; j<DRUM_POLYPHONY; j++ ) {
        if ( soundOrder_[j] > soundOrder_[iWave] )
          soundOrder_[j] -= 1;
      }
    }
    soundOrder_[iWave] = nSounding_ - 1;
    soundNumber_[iWave] = noteNumber;

    // Concatenate the STK rawwave path to the rawwave file
    waves_[iWave].openFile( (Stk::rawwavePath() + waveNames[ noteNumber ]).c_str(), true );
    if ( Stk::sampleRate() != 22050.0 )
      waves_[iWave].setRate( 22050.0 / Stk::sampleRate() );
    filters_[iWave].setPole( 0.999 - (amplitude * 0.6) );
    filters_[iWave].setGain( amplitude );
  }

}

void inDrummer2 :: noteOff( StkFloat amplitude )
{
  // Set all sounding wave filter gains low.
  int i = 0;
  while ( i < nSounding_ ) filters_[i++].setGain( amplitude * 0.01 );
}

} // stk namespace
