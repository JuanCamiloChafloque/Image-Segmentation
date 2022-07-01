#include "Semilla.h"

Semilla::Semilla()
{
}

unsigned short &Semilla::getX()
{
    return this->x;
}

void Semilla::setX(unsigned short &pX)
{
    this->x = pX;
}

unsigned short &Semilla::getY()
{
    return this->y;
}

void Semilla::setY(unsigned short &pY)
{
    this->y = pY;
}

unsigned short &Semilla::getL()
{
    return this->l;
}

void Semilla::setL(unsigned short &pL)
{
    this->l = pL;
}