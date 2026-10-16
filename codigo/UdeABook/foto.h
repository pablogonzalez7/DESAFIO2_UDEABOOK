
#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "fecha.h"

using namespace std;

class Foto
{
private:
    string ruta, descripcion;
    Fecha fecha_publicacion;

public:
    Foto();

    Foto(string ruta,
         string descripcion,
         Fecha fecha_publicacion);

    Foto(const Foto& otra);

    ~Foto();

    const Foto& operator=(const Foto& otra);
    bool operator==(const Foto& otra) const;

    void set_datos(string ruta,
                   string descripcion,
                   Fecha fecha_publicacion);

    string get_ruta() const;
    string get_descripcion() const;
    Fecha get_fecha_publicacion() const;
};

#endif
