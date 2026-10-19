#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "fecha.h"

using namespace std;

class Foto
{
private:
    string ruta, descripcion;
    Fecha fecha_captura;
    unsigned int tamano;
    unsigned int likes;

public:
    Foto();

    Foto(const string &ruta, const string &descripcion, Fecha fecha_captura, unsigned int tamano = 0,
         unsigned int likes = 0);

    Foto(const Foto &otra);

    ~Foto();

    const Foto &operator=(const Foto &otra);
    bool operator==(const Foto &otra) const;

    void set_datos(const string &ruta, const string &descripcion, Fecha fecha_captura, unsigned int tamano = 0,
                   unsigned int likes = 0);

    bool set_tamano(unsigned int tamano);
    bool set_likes(unsigned int likes);

    const string &get_ruta() const;
    const string &get_descripcion() const;
    Fecha get_fecha_captura() const;
    Fecha get_fecha_publicacion() const;
    unsigned int get_tamano() const;
    unsigned int get_likes() const;

    void dar_like();
    bool quitar_like();
};

#endif
