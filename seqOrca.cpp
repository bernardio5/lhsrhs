
#include "seqOrca.h"

// 36 numberletters containing values 0-35. OBOBs gaLORE!!

void seqOrca::setC(char ch) {
    f = CtoF(ch); 
    if (f<0.01) {   c = '0'; } // illegal chars map to 0
    else {          c = ch;  } 
}

void seqOrca::setF(float x) { 
    f = x;
    if (f<0.0) { f = 0.0f; } // out of range => clamp
    if (f>1.0) { f = 1.0f; }
    c = FtoC(f); 
}

void seqOrca::inc() { // maybe this will work? we'll see! 
    setC(c+1);
}

bool seqOrca::dec() { // subtract 1/36 from f, which decreases the displayed val by 1
    bool res = false; 
    float newf = f - 0.028571429;  
    if ((f>0.01f) && (newf < 0.01f)) { // if dropping to 0
      res = true; 
      newf = 0.0f;
    }
    setF(newf); 
    return res; 
}

float seqOrca::CtoF(char in) { 
    return CtoI(in)/35.0; 
}

char seqOrca::FtoC(float in) { 
    return ItoC((int)round(in*35.0)); 
}

char seqOrca::ItoC(int in) {
            //   012345678901234567890123456789012345   z=chars[35]
            //             1         2         3        val(z)=35
    char cs[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (in<0) { return cs[0]; } // out of range => clamp
    if (in>35) { return cs[35]; }
    return cs[in];
}

int seqOrca::CtoI(char in) { 
    char cs[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    bool fnd=false;
    int res=0, i=0; 
    while ((i<36) && (!fnd)) {
        if (cs[i]==in) { fnd=true; res=i; }
        ++i; 
    }
    if (fnd) { return res; }
    return 0; 
}    


/*
    // testing code
    seqOrca a;
    int i,j,k;
    char c1, c2;
    float f;
    char cs[] = "ASD0123456789abcdefghijklmnopqrstuvwxyzRGW";
    for (i=0; i<42; ++i) {
        c1 = cs[i]; 
        f = .028571429 * i;
        a.initC(c1);
        printf("i=%d ItoC=%c char[i]=%c a.c=%c a.f=%f\n", i, seqOrca::ItoC(i), c1, a.c, a.f);
        printf("   FtoC:%c  CtoI:%d  CtoF:%f", seqOrca::FtoC(a.f), seqOrca::CtoI(c1), seqOrca::CtoF(c1));
        a.initF(f); 
        printf("   a.initF:%c %f \n", a.c,a.f);
    }

*/
