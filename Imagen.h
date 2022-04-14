#ifndef IMAGEN_H_
#define IMAGEN_H_

#include <string>
#include <vector>
#include <iostream>

class Imagen{

  protected:
    std::string nombre;
    unsigned short H;
    unsigned short W;
    unsigned short M;
    std::vector<std::vector<unsigned short>> pixeles;

  public:
    Imagen();
    const unsigned short& getH() const;
    const unsigned short& getW() const;
    const unsigned short& getM() const;
    const std::string& getNombre() const;
    void inicializar( );
    bool leerImagen( std::string archivo_ppm );
    void setPixel( unsigned short& i, unsigned short& j, unsigned short& p );
    unsigned short& getPixel( unsigned short& row, unsigned short& col );
    std::vector<unsigned short>& operator[]( const unsigned int& i );
    void setH( const unsigned short& pH );
    void setW( const unsigned short& pW );
    void setM( const unsigned short& pM );
    void setNombre( const std::string& pNombre );

};

#include "imagen.cpp"

#endif
