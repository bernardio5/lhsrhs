#pragma once

#include "seqOrca.h"


class seqKeyable {

public:
    seqKeyable() {}

    void init(float initVal);
    
    void editC(char c) { editV.setC(c); }
    void editF(float f) { editV.setF(f); }

    void key(float dur, float now); 
    void update(float t);

    char getEditC() { return editV.c; }
    char getCurrentC() { return currentV.c; }

    float val() { return currentV.f; }

    seqOrca editV, currentV; // displayed
    float startV, goalV, startT, goalT; // hidden; simplify
    bool changing, inert; 
};


/* test code

    
    seqKeyable d; 
    float t; 
    char cs[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char nc; 

    int ctr = 0;
    for (int i=0; i<36; ++i) { 
        d.init( i/35.0 );
        printf("i:%d d.val:%f  ---  ", i, d.val());
        if(ctr%2==0) { printf("\n"); }
        ++ctr;
    }

    d.init( 0.0 );
    d.editC('z');
    d.key(1.0, 0.0);
    for (t=0.0; t<1.0; t=t+0.01) { 
        d.update(t);
        printf("1 t:%f d.val:%f d.c:%c  +++  ", t, d.val(), d.currentV.c);
        if(ctr%2==0) { printf("\n"); }
        ++ctr;
    }

    d.editF(0.0); 
    d.key(2.0,1.0);
    for (t=1.0; t<3.0; t=t+0.01) { 
        d.update(t);
        printf("2 t:%f d.val:%f d.c:%c   --- ", t, d.val(), d.currentV.c);
        if(ctr%2==0) { printf("\n"); }
        ++ctr;
    }


*/