#include "seqKeyable.h"


void seqKeyable::init(float initVal) { 
    currentV.initF(initVal); // val as of last update (shown on right)
    editV.initF(initVal);  // editable; copied to goal when keyed (shown on left)
    startV = initVal; // used for interpolation 
    goalV = initVal;
    startT = -1.0; 
    goalT = -1.0;
    changing = false;   // won't change until keyed
    inert = false;      // hasn't been updated since it stopped changing
}


void seqKeyable::key(float duration, float now) { 
    if ((duration<0.001) || (currentV.c==editV.c)) {
      goalT = now; 
      currentV.setF(editV.f);
      changing = false; 
      inert = false; 
    } else {
        if (editV.f != currentV.f) { 
            startV = currentV.f;  // current becomes start
            goalV = editV.f;      // edit copied to goal
            startT = now;                  // reset lerp 
            goalT = now + duration;
            changing = true;
            inert = false;   
            update(now);
        } else {
            changing = false; 
            inert = false; 
        }
    }
}



void seqKeyable::update(float t) { 
    if (changing) {
        if (t<goalT) {
            float fract = (t - startT) / (goalT - startT);
            currentV.setF(startV + (fract * (goalV-startV)));
        } else {
            currentV.setF(editV.f);
            changing = false; 
        } 
    } else { 
        inert = true; 
    }
}
