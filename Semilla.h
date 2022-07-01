#ifndef SEMILLA_H_
#define SEMILLA_H_

class Semilla
{

protected:
    unsigned short x;
    unsigned short y;
    unsigned short l;

public:
    Semilla();
    unsigned short &getX();
    void setX(unsigned short &pX);
    unsigned short &getY();
    void setY(unsigned short &pY);
    unsigned short &getL();
    void setL(unsigned short &pL);
};

#include "semilla.cpp"

#endif