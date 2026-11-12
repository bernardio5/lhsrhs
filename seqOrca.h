#pragma once

#include <cmath>
#include <string>

class seqOrca {
public:
    seqOrca() {}
    void initC(char initVal) { setC(initVal); }
    void initF(float initVal) { setF(initVal); }

    void setC(char ch);
    void setF(float x);

    bool unequal(seqOrca *it) { return c != it->c; }
    int intVal() { return CtoI(c); }

    void inc();
    bool dec(); // return t if going to 0

    static float CtoF(char in); 
    static char FtoC(float in);
    static char ItoC(int in);
    static int CtoI(char in);

    char c; 
    float f; 
};
