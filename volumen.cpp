#include "Volumen.h"
#include "Imagen.h"
#include <sstream>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstring>
#include <iterator>
#include <iostream>

Volumen::Volumen() {}

bool Volumen::cargarVolumen(std::string &nombreBase, unsigned short &numImagenes)
{

    bool finalizo = false;

    std::string linea;
    std::string hashtag = "#";
    int i = 1;
    int contImagenes = 0;
    unsigned short contPixeles;
    unsigned short contW;
    unsigned short contH;
    Imagen nuevaImagen;
    int lectDim;
    bool primero;

    while (contImagenes < numImagenes)
    {
        char *archivo = new char[1048];

        std::string nombreImagen;
        std::stringstream ss;
        ss << i;

        std::string imagenActual = ss.str();
        strcpy(archivo, nombreBase.c_str());
        if (i < 10)
        {
            strcat(archivo, "0");
        }

        strcat(archivo, imagenActual.c_str());

        std::stringstream s(archivo);
        s >> nombreImagen;

        strcat(archivo, ".pgm");

        std::ifstream file(archivo);
        if (file.is_open())
        {
            primero = false;
            lectDim = 0;
            contH = 0;
            contPixeles = 0;
            contW = 0;

            while (getline(file, linea))
            {
                if (linea.length() > 0)
                {
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

                                std::stringstream AnchoLargo(linea);
                                std::string separado;
                                while (AnchoLargo >> separado)
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

                                nuevaImagen.setNombre(nombreImagen);
                                nuevaImagen.setH(lar);
                                nuevaImagen.setM(maxC);
                                nuevaImagen.setW(anc);
                                nuevaImagen.inicializar();
                                lectDim++;
                            }
                            else
                            {
                                char *color = new char[5];

                                std::stringstream dividir(linea);
                                std::string separado;

                                while (dividir >> separado)
                                {
                                    strcpy(color, separado.c_str());
                                    std::stringstream conv(color);
                                    unsigned short P;
                                    conv >> P;

                                    nuevaImagen.setPixel(contH, contW, P);
                                    contW++;

                                    if (contW == nuevaImagen.getW())
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
            }
            file.close();
            finalizo = true;
            std::cout << "\nLei imagen " << contImagenes + 1 << std::endl;
        }
        else
        {
            std::cout << "\nArchivo " << contImagenes + 1 << " invalido. " << std::endl;
        }

        setImagen(nuevaImagen);
        i++;
        contImagenes++;
    }

    return finalizo;
}

bool Volumen::proyeccion2DX(char &criterio, std::string &pArchivo)
{

    bool finalizo = false;
    int Y = imagenes.begin()->getW();
    int Z = imagenes.size();

    unsigned short imagenActual = 0;
    unsigned short mayor;
    unsigned short menor;
    unsigned short prom;
    std::vector<unsigned short> med;

    Imagen proyeccion;
    proyeccion.setH(Y);
    proyeccion.setW(Z);
    proyeccion.inicializar();

    std::list<Imagen>::reverse_iterator itr;
    for (itr = imagenes.rbegin(); itr != imagenes.rend(); itr++)
    {
        for (unsigned short j = 0; j < itr->getW(); j++)
        {
            mayor = 0;
            menor = 257;
            prom = 0;
            med.clear();

            for (unsigned short i = 0; i < itr->getH(); i++)
            {
                if (criterio == 'M')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    if (actual > mayor)
                    {
                        mayor = actual;
                    }
                }
                else if (criterio == 'N')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    if (actual < menor)
                    {
                        menor = actual;
                    }
                }
                else if (criterio == 'P')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    prom += actual;
                }
                else
                {
                    unsigned short actual = itr->getPixel(i, j);
                    med.push_back(actual);
                }
            }

            if (criterio == 'M')
            {
                proyeccion.setPixel(imagenActual, j, mayor);
            }
            else if (criterio == 'N')
            {
                proyeccion.setPixel(imagenActual, j, menor);
            }
            else if (criterio == 'P')
            {
                unsigned short promFinal = prom / itr->getH();
                proyeccion.setPixel(imagenActual, j, promFinal);
            }
            else
            {
                unsigned short medFinal = calcularMediana(med);
                proyeccion.setPixel(imagenActual, j, medFinal);
            }
        }
        imagenActual++;
    }

    std::ofstream proyeccionX;
    bool primero = false;
    int mayorM = 0;
    for (unsigned short i = 0; i < Z; i++)
    {
        for (unsigned short j = 0; j < Y; j++)
        {
            unsigned short actual = proyeccion.getPixel(i, j);
            if (actual > mayorM)
            {
                mayorM = actual;
            }
        }
    }
    proyeccionX.open(pArchivo, std::ios::out);
    proyeccionX << "P2" << std::endl;
    proyeccionX << Y << " " << Z << std::endl;
    proyeccionX << mayorM << std::endl;
    for (unsigned short a = 0; a < Z; a++)
    {
        if (primero)
            proyeccionX << std::endl;
        for (unsigned short z = 0; z < Y; z++)
        {
            unsigned short actual = proyeccion.getPixel(a, z);
            proyeccionX << actual << " ";
            primero = true;
        }
    }
    proyeccionX.close();
    finalizo = true;

    return finalizo;
}

bool Volumen::proyeccion2DY(char &criterio, std::string &pArchivo)
{

    bool finalizo = false;
    int X = imagenes.begin()->getH();
    int Z = imagenes.size();

    unsigned short imagenActual = 0;
    unsigned short mayor;
    unsigned short menor;
    unsigned short prom;
    std::vector<unsigned short> med;

    Imagen proyeccion;
    proyeccion.setH(X);
    proyeccion.setW(Z);
    proyeccion.inicializar();

    std::list<Imagen>::reverse_iterator itr;
    for (itr = imagenes.rbegin(); itr != imagenes.rend(); itr++)
    {
        for (unsigned short i = 0; i < itr->getH(); i++)
        {
            mayor = 0;
            menor = 257;
            prom = 0;
            med.clear();

            for (unsigned short j = 0; j < itr->getW(); j++)
            {
                if (criterio == 'M')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    if (actual > mayor)
                    {
                        mayor = actual;
                    }
                }
                else if (criterio == 'N')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    if (actual < menor)
                    {
                        menor = actual;
                    }
                }
                else if (criterio == 'P')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    prom += actual;
                }
                else
                {
                    unsigned short actual = itr->getPixel(i, j);
                    med.push_back(actual);
                }
            }

            if (criterio == 'M')
            {
                proyeccion.setPixel(imagenActual, i, mayor);
            }
            else if (criterio == 'N')
            {
                proyeccion.setPixel(imagenActual, i, menor);
            }
            else if (criterio == 'P')
            {
                unsigned short promFinal = prom / itr->getH();
                proyeccion.setPixel(imagenActual, i, promFinal);
            }
            else
            {
                unsigned short medFinal = calcularMediana(med);
                proyeccion.setPixel(imagenActual, i, medFinal);
            }
        }
        imagenActual++;
    }

    std::ofstream proyeccionY;
    bool primero = false;
    int mayorM = 0;
    for (unsigned short i = 0; i < Z; i++)
    {
        for (unsigned short j = 0; j < X; j++)
        {
            unsigned short actual = proyeccion.getPixel(i, j);
            if (actual > mayorM)
            {
                mayorM = actual;
            }
        }
    }
    proyeccionY.open(pArchivo, std::ios::out);
    proyeccionY << "P2" << std::endl;
    proyeccionY << Z << " " << X << std::endl;
    proyeccionY << mayorM << std::endl;
    for (unsigned short a = 0; a < X; a++)
    {
        if (primero)
            proyeccionY << std::endl;
        for (unsigned short z = 0; z < Z; z++)
        {
            unsigned short actual = proyeccion.getPixel(z, a);
            proyeccionY << actual << " ";
            primero = true;
        }
    }
    proyeccionY.close();
    finalizo = true;
    return finalizo;
}

bool Volumen::proyeccion2DZ(char &criterio, std::string &pArchivo)
{

    bool finalizo = false;
    int X = imagenes.begin()->getH();
    int Y = imagenes.begin()->getW();

    unsigned short imagenActual = 0;
    unsigned short mayor;
    unsigned short menor;
    unsigned short prom;
    std::vector<unsigned short> med;

    Imagen proyeccion;
    proyeccion.setH(X);
    proyeccion.setW(Y);
    proyeccion.inicializar();

    for (unsigned short i = 0; i < X; i++)
    {
        for (unsigned short j = 0; j < Y; j++)
        {
            std::list<Imagen>::iterator itr = imagenes.begin();
            for (; itr != imagenes.end(); itr++)
            {
                if (criterio == 'M')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    if (actual > mayor)
                    {
                        mayor = actual;
                    }
                }
                else if (criterio == 'N')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    if (actual < menor)
                    {
                        menor = actual;
                    }
                }
                else if (criterio == 'P')
                {
                    unsigned short actual = itr->getPixel(i, j);
                    prom += actual;
                }
                else
                {
                    unsigned short actual = itr->getPixel(i, j);
                    med.push_back(actual);
                }
            }

            if (criterio == 'M')
            {
                proyeccion.setPixel(i, j, mayor);
                mayor = 0;
            }
            else if (criterio == 'N')
            {
                proyeccion.setPixel(i, j, menor);
                menor = 257;
            }
            else if (criterio == 'P')
            {
                unsigned short promFinal = prom / imagenes.size();
                proyeccion.setPixel(i, j, promFinal);
                prom = 0;
            }
            else
            {
                unsigned short medFinal = calcularMediana(med);
                proyeccion.setPixel(i, j, medFinal);
                med.clear();
            }
        }
    }

    std::ofstream proyeccionZ;
    bool primero = false;
    int mayorM = 0;
    for (unsigned short i = 0; i < X; i++)
    {
        for (unsigned short j = 0; j < Y; j++)
        {
            unsigned short actual = proyeccion.getPixel(i, j);
            if (actual > mayorM)
            {
                mayorM = actual;
            }
        }
    }
    proyeccionZ.open(pArchivo, std::ios::out);
    proyeccionZ << "P2" << std::endl;
    proyeccionZ << Y << " " << X << std::endl;
    proyeccionZ << mayorM << std::endl;
    for (unsigned short a = 0; a < X; a++)
    {
        if (primero)
            proyeccionZ << std::endl;
        for (unsigned short z = 0; z < Y; z++)
        {
            unsigned short actual = proyeccion.getPixel(a, z);
            proyeccionZ << actual << " ";
            primero = true;
        }
    }
    proyeccionZ.close();
    finalizo = true;

    return finalizo;
}

unsigned short Volumen::calcularMediana(std::vector<unsigned short> &mediana)
{
    unsigned short N = mediana.size();
    unsigned short aux;

    for (unsigned short i = 0; i < N; i++)
    {
        for (unsigned short j = i + 1; j < N; j++)
        {
            if (mediana[i] > mediana[j])
            {
                aux = mediana[i];
                mediana[i] = mediana[j];
                mediana[j] = aux;
            }
        }
    }

    if (N % 2 == 0)
    {
        return (mediana[N / 2] + mediana[(N / 2) - 1]) / 2;
    }
    else
    {
        return mediana[(N / 2)];
    }
}

const std::list<Imagen> &Volumen::getImagenes() const
{
    return this->imagenes;
}

const std::string &Volumen::getNombre() const
{
    return this->nombre;
}

void Volumen::setImagen(const Imagen &pImagen)
{
    this->imagenes.push_back(pImagen);
}

void Volumen::setNombre(std::string &pNombre)
{
    this->nombre = pNombre;
}
