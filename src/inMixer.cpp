/***************************************************/
/*! \class inMixer
    
*/
/***************************************************/

#include "inMixer.h"
#include <cmath>


namespace stk {

  inMixer::inMixer() {
    for (int i=0; i<SEQ_ROW_CT; ++i) {
      insts[i] = 0; 
    }
  }

  bool inMixer::inRange(int i) { return ((-1<i)&&(i<SEQ_ROW_CT)); }


  // TBD: all instruments should make only one channel. 
  void inMixer::setInst(int which, Instrmnt *inst) {
    if (inRange(which)) {
      insts[which] = inst; // old one is deleted in row.cpp

      // Check output channels and resize lastFrame_ if necessary.
      unsigned int maxChannels = 1;
      for (int i=0; i<SEQ_ROW_CT; ++i ) {
        if (insts[i]!=0) {
          if (insts[i]->channelsOut() > maxChannels ) { 
            maxChannels = insts[i]->channelsOut();
          } 
        } 
      }
      if ( maxChannels != lastFrame_.channels() ) {
        unsigned int startChannel = lastFrame_.channels();
        lastFrame_.resize( 1, maxChannels );
        for ( unsigned int i=startChannel; i<maxChannels; i++ ) {
          lastFrame_[i] = 0.0;
        }
      }

    } // else error
  }

} // stk namespace
