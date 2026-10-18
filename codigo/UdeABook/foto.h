
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
    long long tamano;
    int likes;

public:
    Foto();

    Foto(string ruta, string descripcion, Fecha fecha_captura,
         long long tamano = 0, int likes = 0);

    Foto(const Foto& otra);

    ~Foto();

    const Foto& operator=(const Foto& otra);
    bool operator==(const Foto& otra) const;

    void set_datos(string ruta, string descripcion,
                   Fecha fecha_captura,
                   long long tamano = 0, int likes = 0);

    bool set_tamano(long long tamano);
    bool set_likes(int likes);

    string get_ruta() const;
    string get_descripcion() const;
    Fecha get_fecha_captura() const;
    Fecha get_fecha_publicacion() const;
    long long get_tamano() const;
    int get_likes() const;

    void dar_like();
    bool quitar_like();
};

#endif
