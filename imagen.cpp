#include <iostream>
#include <string.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <string>
#include "Imagen.h"

Imagen::Imagen() {}

bool Imagen::leerImagen(std::string archivo_ppm)
{

  std::string linea;
  std::string hashtag = "#";
  unsigned short contW;
  unsigned short contH;

  int contLinea = 1;
  int lectDim;
  bool primero;
  bool finalizo = false;
  char *archivo = new char[1048];

  strcpy(archivo, archivo_ppm.c_str());
  std::ifstream file(archivo);
  if (file.is_open())
  {
    primero = false;
    lectDim = 0;
    contH = 0;
    contW = 0;

    while (getline(file, linea))
    {
      if (linea.length() == 0)
      {
        continue;
      }

      if (primero)
      {
        char *copia = new char[linea.length() + 1];
        strcpy(copia, linea.c_str());
        char esHashtag = *(copia + 0);

        if (esHashtag != '#')
        {
          // SE LEE EL ANCHO Y EL LARGO DE LA IMAGEN
          if (lectDim == 0)
          {
            char *ancho = new char[linea.length() + 1];
            char *largo = new char[linea.length() + 1];
            int contAncho = 0;

            std::stringstream anchoLargo(linea);
            std::string separado;
            while (anchoLargo >> separado)
            {
              if (contAncho == 0)
              {
                strcpy(ancho, separado.c_str());
                contAncho++;
              }
              else
              {
                strcpy(largo, separado.c_str());
              }
            }

            getline(file, linea);
            char *maxColor = new char[linea.length() + 1];
            strcpy(maxColor, linea.c_str());

            std::stringstream a(ancho);
            unsigned short anc;
            a >> anc;
            std::stringstream l(largo);
            unsigned short lar;
            l >> lar;
            std::stringstream m(maxColor);
            unsigned short maxC;
            m >> maxC;

            setNombre(archivo_ppm);
            setH(lar);
            setM(maxC);
            setW(anc);
            inicializar();
            lectDim++;
          }
          else
          {
            char *color = new char[1048];
            char *stream = new char[linea.length() + 1];
            strcpy(stream, linea.c_str());

            std::stringstream dividir(stream);
            std::string separado;

            while (dividir >> separado)
            {
              strcpy(color, separado.c_str());
              std::stringstream conv(color);
              unsigned short P;
              conv >> P;

              setPixel(contH, contW, P);
              contW++;

              if (contW == getW())
              {
                contW = 0;
                contH++;
              }
            }
          }
        }
      }
      primero = true;
    }
    file.close();
    finalizo = true;
  }
  else
  {
    std::cout << "\nArchivo invalido. " << std::endl;
  }

  return finalizo;
}

void Imagen::inicializar()
{
  this->pixeles = std::vector<std::vector<unsigned short>>(this->H, std::vector<unsigned short>(this->W, 0));
  for (int i = 0; i < this->H; i++)
  {
    for (int j = 0; j < this->W; j++)
    {
      double max = 100000;
      this->distancias.push_back(max);
    }
  }
}

void Imagen::setPixel(unsigned short &i, unsigned short &j, unsigned short &p)
{
  (*this)[i][j] = p;
}

std::vector<unsigned short> &Imagen::operator[](const unsigned int &i)
{
  return this->pixeles[i];
}

unsigned short &Imagen::getPixel(unsigned short &row, unsigned short &col)
{
  return this->pixeles[row][col];
}

const unsigned short &Imagen::getH() const
{
  return this->H;
}

const unsigned short &Imagen::getW() const
{
  return this->W;
}

const unsigned short &Imagen::getM() const
{
  return this->M;
}

const std::string &Imagen::getNombre() const
{
  return this->nombre;
}

void Imagen::setH(const unsigned short &pH)
{
  this->H = pH;
}

void Imagen::setW(const unsigned short &pW)
{
  this->W = pW;
}

void Imagen::setM(const unsigned short &pM)
{
  this->M = pM;
}

void Imagen::setNombre(const std::string &pNombre)
{
  this->nombre = pNombre;
}

void Imagen::setDistancia(int index, double val)
{
  this->distancias[index] = val;
}

double Imagen::getDistancia(int index)
{
  return this->distancias[index];
}
