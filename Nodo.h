#ifndef NODO_H_
#define NODO_H_

#include <map>

class Nodo
{

protected:
    unsigned short frecuency;
    int simbolo;
    Nodo *hijoIzq;
    Nodo *hijoDer;

public:
    Nodo();
    const int &getSimbolo() const;
    const unsigned short &getFrecuency() const;
    void setSimbolo(int pSimbolo);
    void setFrecuency(unsigned short pFrecuency);
    void setHijoIzq(Nodo *hijoIzq);
    void setHijoDer(Nodo *hijoDer);
    Nodo *getHijoIzq();
    Nodo *getHijoDer();
    bool esHoja();
};

#include "nodo.cpp"

#endif