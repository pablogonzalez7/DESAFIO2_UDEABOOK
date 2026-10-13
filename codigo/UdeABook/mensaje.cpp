#include "mensaje.h"

using namespace std;

Mensaje::Mensaje()
{
    this->contenido = "";
    this->emisor = nullptr;
    this->receptor = nullptr;
    this->leido = false;
}

Mensaje::Mensaje(string contenido, Usuario* emisor, Usuario* receptor)
{
    this->contenido = contenido;
    this->emisor = emisor;
    this->receptor = receptor;
    this->leido = false;
}

string Mensaje::get_contenido() const
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

bool Mensaje::get_leido() const
{
    return this->leido;
}

void Mensaje::marcar_leido()
{
    this->leido = true;
}
