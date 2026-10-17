#include "mensaje.h"

Mensaje::Mensaje()
{
    this->contenido = "";
    this->emisor = nullptr;
    this->receptor = nullptr;
    this->leido = false;
}

Mensaje::Mensaje(string contenido, Usuario* emisor,
                 Usuario* receptor, const Fecha& fecha,
                 bool leido)
{
    this->contenido = contenido;
    this->emisor = emisor;
    this->receptor = receptor;
    this->fecha = fecha;
    this->leido = leido;
}

Mensaje::Mensaje(const Mensaje& otro)
{
    this->contenido = otro.contenido;
    this->emisor = otro.emisor;
    this->receptor = otro.receptor;
    this->fecha = otro.fecha;
    this->leido = otro.leido;
}

const Mensaje& Mensaje::operator=(const Mensaje& otro)
{
    if (this != &otro)
    {
        this->contenido = otro.contenido;
        this->emisor = otro.emisor;
        this->receptor = otro.receptor;
        this->fecha = otro.fecha;
        this->leido = otro.leido;
    }

    return *this;
}

bool Mensaje::operator==(const Mensaje& otro) const
{
    return this->contenido == otro.contenido &&
           this->emisor == otro.emisor &&
           this->receptor == otro.receptor &&
           this->fecha.get_dia() == otro.fecha.get_dia() &&
           this->fecha.get_mes() == otro.fecha.get_mes() &&
           this->fecha.get_anio() == otro.fecha.get_anio() &&
           this->leido == otro.leido;
}

const string& Mensaje::get_contenido() const
{
    return this->contenido;
}

Usuario* Mensaje::get_emisor() const
{
    return this->emisor;
}

Usuario* Mensaje::get_receptor() const
{
    return this->receptor;
}

const Fecha& Mensaje::get_fecha() const
{
    return this->fecha;
}

bool Mensaje::get_leido() const
{
    return this->leido;
}

void Mensaje::set_datos(string contenido, Usuario* emisor,
                        Usuario* receptor, const Fecha& fecha,
                        bool leido)
{
    this->contenido = contenido;
    this->emisor = emisor;
    this->receptor = receptor;
    this->fecha = fecha;
    this->leido = leido;
}

void Mensaje::marcar_leido()
{
    this->leido = true;
}
