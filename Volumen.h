#ifndef VOLUMEN_H_
#define VOLUMEN_H_
#include <list>
#include <vector>
#include "Imagen.h"

class Volumen {

  protected:
    std::string nombre;
    std::list< Imagen > imagenes;

  public:
    Volumen();
    const std::string& getNombre() const;
    void setNombre( std::string& pNombre );
    const std::list< Imagen >& getImagenes() const;
    void setImagen( const Imagen& pImagen );
    bool proyeccion2DX( char& criterio, std::string& pArchivo );
    bool proyeccion2DY( char& criterio, std::string& pArchivo );
    bool proyeccion2DZ( char& criterio, std::string& pArchivo );
    unsigned short calcularMediana( std::vector< unsigned short >& mediana );
    bool cargarVolumen( std::string& nombreBase, unsigned short& numImagenes );
};

#include "volumen.cpp"

#endif
