
#include "foto.h"

Foto::Foto()
{
    this->ruta = "";
    this->descripcion = "";
    this->tamano = 0;
    this->likes = 0;
}

Foto::Foto(string ruta, string descripcion,
           Fecha fecha_captura, long long tamano, int likes)
{
    this->set_datos(ruta, descripcion, fecha_captura,
                    tamano, likes);
}

Foto::Foto(const Foto& otra)
{
    this->ruta = otra.ruta;
    this->descripcion = otra.descripcion;
    this->fecha_captura = otra.fecha_captura;
    this->tamano = otra.tamano;
    this->likes = otra.likes;
}

Foto::~Foto()
{
}

const Foto& Foto::operator=(const Foto& otra)
{
    if (this != &otra) {
        this->ruta = otra.ruta;
        this->descripcion = otra.descripcion;
        this->fecha_captura = otra.fecha_captura;
        this->tamano = otra.tamano;
        this->likes = otra.likes;
    }

    return *this;
}

bool Foto::operator==(const Foto& otra) const
{
    return this->ruta == otra.ruta;
}

void Foto::set_datos(string ruta, string descripcion,
                     Fecha fecha_captura,
                     long long tamano, int likes)
{
    this->ruta = ruta;
    this->descripcion = descripcion;
    this->fecha_captura = fecha_captura;
    this->tamano = tamano >= 0 ? tamano : 0;
    this->likes = likes >= 0 ? likes : 0;
}

bool Foto::set_tamano(long long tamano)
{
    if (tamano < 0)
        return false;

    this->tamano = tamano;

    return true;
}

bool Foto::set_likes(int likes)
{
    if (likes < 0)
        return false;

    this->likes = likes;

    return true;
}

string Foto::get_ruta() const
{
    return this->ruta;
}

string Foto::get_descripcion() const
{
    return this->descripcion;
}

Fecha Foto::get_fecha_captura() const
{
    return this->fecha_captura;
}

Fecha Foto::get_fecha_publicacion() const
{
    return this->fecha_captura;
}

long long Foto::get_tamano() const
{
    return this->tamano;
}

int Foto::get_likes() const
{
    return this->likes;
}

void Foto::dar_like()
{
    this->likes++;
}

bool Foto::quitar_like()
{
    if (this->likes == 0)
        return false;

    this->likes--;

    return true;
}
