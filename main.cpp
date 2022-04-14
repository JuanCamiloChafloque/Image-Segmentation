#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <cstring>
#include "Imagen.h"
#include "Volumen.h"

int main( int argc, char* argv[] ){

  char* entrada = new char[1048];
  bool cargoVolumen = false;
  bool cargoImagen = false;
  Volumen volumen;
  Imagen imagen;

  while(entrada != "salir") {

    std::cout<<"\n$ ";
    std::cin.getline( entrada, 1048);

    char* copia = new char[1048];
    strcpy(copia, entrada );

    char* comando = strtok(copia, " ");

    if( strcmp( comando, "cargar_imagen" ) == 0 ) {
      char* nombre_imagen = new char[1048];

      comando = strtok(NULL, " ");
      strcpy(nombre_imagen, comando);

      std::stringstream nomImagen( nombre_imagen );
      std::string nom_imagen;
      nomImagen >> nom_imagen;

      cargoImagen = imagen.leerImagen( nom_imagen );
      if( cargoImagen ) {
        std::cout<<"\nLa imagen se cargo exitosamente. "<<std::endl;
      }
      
    }
    else if( strcmp( comando, "cargar_volumen" ) == 0 ){

      char* nombre_base = new char[1048];
      char* num_imagenes = new char[1048];
      
      comando = strtok(NULL, " ");
      strcpy(nombre_base, comando);
      comando = strtok(NULL, " ");
      strcpy(num_imagenes,comando);

      std::stringstream numImagenes( num_imagenes );
      unsigned short numImagen;
      numImagenes >> numImagen;

      std::stringstream nomBase( nombre_base );
      std::string nom_base;
      nomBase >> nom_base;

      if( numImagen > 0 && numImagen < 100 ){
        volumen.setNombre( nom_base );
        cargoVolumen = volumen.cargarVolumen( nom_base,  numImagen ) ;
        if( cargoVolumen ){
          std::cout<<"\nEl volumen se cargo exitosamente. "<<std::endl;
        }
      } else {
          std::cout<<"\nNumero de imagenes invalido. "<<std::endl;
      }
    }
    else if( strcmp( comando, "info_imagen" ) == 0 ) {
      if(cargoImagen) {
        std::cout<<"\nNombre de la imagen: "<<imagen.getNombre()<<std::endl;
        std::cout<<"\nAncho de la imagen: "<<imagen.getW()<<std::endl;
        std::cout<<"\nAlto de la imagen: "<<imagen.getH()<<std::endl;

      } else {
          std::cout<<"\nNo se ha cargado una imagen. "<<std::endl;
      }
    }
    else if( strcmp( comando, "info_volumen" ) == 0 ) {
      if(cargoVolumen) {
        std::cout<<"\nNombre base del volumen: "<<volumen.getNombre()<<std::endl;
        std::cout<<"\nCantidad de imagenes: "<<volumen.getImagenes().size()<<std::endl;
        std::cout<<"\nAncho de las imagenes: "<<volumen.getImagenes().begin()->getW()<<std::endl;
        std::cout<<"\nAlto de las imagenes: "<<volumen.getImagenes().begin()->getH()<<std::endl;

      } else {
          std::cout<<"\nNo se ha cargado un volumen. "<<std::endl;
      }
    }
    else if( strcmp( comando, "proyeccion2D" ) == 0 ){

      char* direccion = new char[1048];
      char* criterio = new char[1048];
      char* nom_archivo = new char[1048];

      comando = strtok( NULL,  " " );
      strcpy(direccion, comando );
      comando = strtok( NULL, " " );
      strcpy(criterio, comando);
      comando = strtok( NULL, " " );
      strcpy(nom_archivo, comando);
 
      if( cargoVolumen ){
        std::stringstream dir( direccion );
        std::string direccion_proy;
        dir >> direccion_proy;

        std::stringstream crit( criterio );
        char criterio_proy;
        crit >> criterio_proy;

        std::stringstream arch( nom_archivo );
        std::string archivo_ppm;
        arch >> archivo_ppm;

        if(direccion_proy != "X" && direccion_proy != "Y" && direccion_proy != "Z"){
          std::cout<<"\nCoordenada invalida. "<<std::endl;

        //M: Maximo, N: Minimo, P: Promedio, E: Mediana
        }else if(criterio_proy != 'M' && criterio_proy != 'N' && criterio_proy != 'P' && criterio_proy != 'E'){
          std::cout<<"\nCriterio invalido. "<<std::endl;
        }
        else {
            if(direccion_proy == "X"){
                bool finalizo = volumen.proyeccion2DX( criterio_proy , archivo_ppm );
                if( finalizo ){
                  std::cout<<"\nProyeccion realizada correctamente. "<<std::endl;
                }

            }else if(direccion_proy == "Y"){
                bool finalizo = volumen.proyeccion2DY( criterio_proy, archivo_ppm );
                if( finalizo ){
                  std::cout<<"\nProyeccion realizada correctamente. "<<std::endl;
                }

            }else {
                bool finalizo = volumen.proyeccion2DZ( criterio_proy, archivo_ppm );
                if( finalizo ){
                  std::cout<<"\nProyeccion realizada correctamente. "<<std::endl;
                }
            }
        }
      }else{
        std::cout<<"\nNo se ha cargado el volumen. "<<std::endl;
      }  

    }
    else if( strcmp( comando, "help" ) == 0 ){
      std::cout<<"\nComandos Disponibles: "<<std::endl;
      std::cout<<"\n cargar_imagen <nombre_imagen> ";
      std::cout<<"\n cargar_volumen <nombre_base> <n_im> ";
      std::cout<<"\n info_imagen ";
      std::cout<<"\n info_volumen ";
      std::cout<<"\n proyeccion2D <direccion> <criterio> <nombre_archivo.ppm> ";
      std::cout<<"\n salir"<<std::endl<<std::endl;

    }
    else if( strcmp( comando, "salir" ) == 0 ) {
      std::cout<<"\nSe ha salido del sistema. "<<std::endl<<std::endl;
      return 0;
    }
    else{
      std::cout<<"\nComando invalido. "<<std::endl<<std::endl;
    }
  }
  return 0;
}