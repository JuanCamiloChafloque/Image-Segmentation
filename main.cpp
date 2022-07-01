#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <cstring>
#include "Imagen.h"
#include "Volumen.h"
#include "HuffmanTree.h"
#include "Graph.h"
#include "Semilla.h"

int main(int argc, char *argv[])
{

  char *entrada = new char[1048];
  bool cargoVolumen = false;
  bool cargoImagen = false;
  Volumen volumen;
  Imagen imagen;

  while (entrada != "salir")
  {

    std::cout << "\n$ ";
    std::cin.getline(entrada, 1048);

    char *copia = new char[1048];
    strcpy(copia, entrada);

    char *comando = strtok(copia, " ");

    if (strcmp(comando, "cargar_imagen") == 0)
    {
      char *nombre_imagen = new char[1048];

      comando = strtok(NULL, " ");
      strcpy(nombre_imagen, comando);

      std::stringstream nomImagen(nombre_imagen);
      std::string nom_imagen;
      nomImagen >> nom_imagen;

      cargoImagen = imagen.leerImagen(nom_imagen);
      if (cargoImagen)
      {
        std::cout << "\nLa imagen se cargo exitosamente. " << std::endl;
      }
    }
    else if (strcmp(comando, "cargar_volumen") == 0)
    {

      char *nombre_base = new char[1048];
      char *num_imagenes = new char[1048];

      comando = strtok(NULL, " ");
      strcpy(nombre_base, comando);
      comando = strtok(NULL, " ");
      strcpy(num_imagenes, comando);

      std::stringstream numImagenes(num_imagenes);
      unsigned short numImagen;
      numImagenes >> numImagen;

      std::stringstream nomBase(nombre_base);
      std::string nom_base;
      nomBase >> nom_base;

      if (numImagen > 0 && numImagen < 100)
      {
        volumen.setNombre(nom_base);
        cargoVolumen = volumen.cargarVolumen(nom_base, numImagen);
        if (cargoVolumen)
        {
          std::cout << "\nEl volumen se cargo exitosamente. " << std::endl;
        }
      }
      else
      {
        std::cout << "\nNumero de imagenes invalido. " << std::endl;
      }
    }
    else if (strcmp(comando, "info_imagen") == 0)
    {
      if (cargoImagen)
      {
        std::cout << "\nNombre de la imagen: " << imagen.getNombre() << std::endl;
        std::cout << "\nAncho de la imagen: " << imagen.getW() << std::endl;
        std::cout << "\nAlto de la imagen: " << imagen.getH() << std::endl;
      }
      else
      {
        std::cout << "\nNo se ha cargado una imagen. " << std::endl;
      }
    }
    else if (strcmp(comando, "info_volumen") == 0)
    {
      if (cargoVolumen)
      {
        std::cout << "\nNombre base del volumen: " << volumen.getNombre() << std::endl;
        std::cout << "\nCantidad de imagenes: " << volumen.getImagenes().size() << std::endl;
        std::cout << "\nAncho de las imagenes: " << volumen.getImagenes().begin()->getW() << std::endl;
        std::cout << "\nAlto de las imagenes: " << volumen.getImagenes().begin()->getH() << std::endl;
      }
      else
      {
        std::cout << "\nNo se ha cargado un volumen. " << std::endl;
      }
    }
    else if (strcmp(comando, "proyeccion2D") == 0)
    {

      char *direccion = new char[1048];
      char *criterio = new char[1048];
      char *nom_archivo = new char[1048];

      comando = strtok(NULL, " ");
      strcpy(direccion, comando);
      comando = strtok(NULL, " ");
      strcpy(criterio, comando);
      comando = strtok(NULL, " ");
      strcpy(nom_archivo, comando);

      if (cargoVolumen)
      {
        std::stringstream dir(direccion);
        std::string direccion_proy;
        dir >> direccion_proy;

        std::stringstream crit(criterio);
        char criterio_proy;
        crit >> criterio_proy;

        std::stringstream arch(nom_archivo);
        std::string archivo_ppm;
        arch >> archivo_ppm;

        if (direccion_proy != "X" && direccion_proy != "Y" && direccion_proy != "Z")
        {
          std::cout << "\nCoordenada invalida. " << std::endl;

          // M: Maximo, N: Minimo, P: Promedio, E: Mediana
        }
        else if (criterio_proy != 'M' && criterio_proy != 'N' && criterio_proy != 'P' && criterio_proy != 'E')
        {
          std::cout << "\nCriterio invalido. " << std::endl;
        }
        else
        {
          if (direccion_proy == "X")
          {
            bool finalizo = volumen.proyeccion2DX(criterio_proy, archivo_ppm);
            if (finalizo)
            {
              std::cout << "\nProyeccion realizada correctamente. " << std::endl;
            }
          }
          else if (direccion_proy == "Y")
          {
            bool finalizo = volumen.proyeccion2DY(criterio_proy, archivo_ppm);
            if (finalizo)
            {
              std::cout << "\nProyeccion realizada correctamente. " << std::endl;
            }
          }
          else
          {
            bool finalizo = volumen.proyeccion2DZ(criterio_proy, archivo_ppm);
            if (finalizo)
            {
              std::cout << "\nProyeccion realizada correctamente. " << std::endl;
            }
          }
        }
      }
      else
      {
        std::cout << "\nNo se ha cargado el volumen. " << std::endl;
      }
    }
    else if (strcmp(comando, "codificar_imagen") == 0)
    {
      if (cargoImagen)
      {
        HuffmanTree *arbol = new HuffmanTree();
        char *nom_archivo = new char[1048];
        comando = strtok(NULL, " ");
        strcpy(nom_archivo, comando);

        std::stringstream arch(nom_archivo);
        std::string archivo_huffman;
        arch >> archivo_huffman;

        arbol->construirArbol(imagen, archivo_huffman);
      }
      else
      {
        std::cout << "\nNo se ha cargado una imagen. " << std::endl;
      }
    }
    else if (strcmp(comando, "decodificar_archivo") == 0)
    {
      HuffmanTree *arbol = new HuffmanTree();
      char *arc_huffman = new char[1048];
      char *arc_ppm = new char[1048];

      comando = strtok(NULL, " ");
      strcpy(arc_huffman, comando);
      comando = strtok(NULL, " ");
      strcpy(arc_ppm, comando);

      std::stringstream arch(arc_huffman);
      std::string archivo_huffman;
      arch >> archivo_huffman;

      std::stringstream arch2(arc_ppm);
      std::string archivo_ppm;
      arch2 >> archivo_ppm;

      arbol->decodificarArchivo(archivo_huffman, archivo_ppm);
    }
    else if (strcmp(comando, "segmentar") == 0)
    {
      if (cargoImagen)
      {
        char *nom_archivo = new char[1048];
        comando = strtok(NULL, " ");
        strcpy(nom_archivo, comando);
        comando = strtok(NULL, " ");

        std::stringstream arch(nom_archivo);
        std::string archivo_ppm;
        arch >> archivo_ppm;

        std::vector<Semilla> semillas;
        Semilla semilla;
        int contSemilla = 0;

        while (comando != NULL)
        {
          std::stringstream seg(comando);
          unsigned short s;
          seg >> s;

          if (contSemilla == 0)
          {
            semilla.setX(s);
            contSemilla++;
          }
          else if (contSemilla == 1)
          {
            semilla.setY(s);
            contSemilla++;
          }
          else if (contSemilla == 2)
          {
            semilla.setL(s);
            contSemilla++;
          }

          if (contSemilla == 3)
          {
            contSemilla = 0;
            semillas.push_back(semilla);
          }

          comando = strtok(NULL, " ");
        }

        Grafo<int, double> *grafo = new Grafo<int, double>(imagen); // Key: Coordenada (Ej. 20,20)
        grafo->crearGrafo(semillas, archivo_ppm);
      }
      else
      {
        std::cout << "\nNo se ha cargado una imagen. " << std::endl;
      }
    }
    else if (strcmp(comando, "help") == 0)
    {
      std::cout << "\nComandos Disponibles: " << std::endl;
      std::cout << "\n cargar_imagen <nombre_imagen> ";
      std::cout << "\n cargar_volumen <nombre_base> <n_im> ";
      std::cout << "\n info_imagen ";
      std::cout << "\n info_volumen ";
      std::cout << "\n proyeccion2D <direccion> <criterio> <nombre_archivo.pgm> ";
      std::cout << "\n codificar_imagen <nombre_archivo.huffman>";
      std::cout << "\n decodificar_archivo <nombre_archivo.huffman> <nombre_imagen.pgm>";
      std::cout << "\n segmentar <salida_imagen.pgm> <semillas>...";
      std::cout << "\n salir" << std::endl
                << std::endl;
    }
    else if (strcmp(comando, "salir") == 0)
    {
      std::cout << "\nSe ha salido del sistema. " << std::endl
                << std::endl;
      return 0;
    }
    else
    {
      std::cout << "\nComando invalido. " << std::endl
                << std::endl;
    }
  }
  return 0;
}