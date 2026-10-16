
#include "foto.h"

Foto::Foto()
{
    this->ruta = "";
    this->descripcion = "";
}

Foto::Foto(string ruta,
           string descripcion,
           Fecha fecha_publicacion)
{
    this->ruta = ruta;
    this->descripcion = descripcion;
    this->fecha_publicacion = fecha_publicacion;
}

Foto::Foto(const Foto& otra)
{
    this->ruta = otra.ruta;
    this->descripcion = otra.descripcion;
    this->fecha_publicacion = otra.fecha_publicacion;
}

Foto::~Foto()
{
}

const Foto& Foto::operator=(const Foto& otra)
{
    if (this != &otra) {
        this->ruta = otra.ruta;
        this->descripcion = otra.descripcion;
        this->fecha_publicacion = otra.fecha_publicacion;
    }

    return *this;
}

bool Foto::operator==(const Foto& otra) const
{
    return this->ruta == otra.ruta;
}

void Foto::set_datos(string ruta,
                     string descripcion,
                     Fecha fecha_publicacion)
{
    this->ruta = ruta;
    this->descripcion = descripcion;
    this->fecha_publicacion = fecha_publicacion;
}

string Foto::get_ruta() const
{
    return this->ruta;
}

string Foto::get_descripcion() const
{
    return this->descripcion;
}

Fecha Foto::get_fecha_publicacion() const
{
    return this->fecha_publicacion;
}
