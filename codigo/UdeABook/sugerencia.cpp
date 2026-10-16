#include "sugerencia.h"

Sugerencia::Sugerencia()
{
    this->usuario = nullptr;
    this->amigos_comunes = 0;
}

Sugerencia::Sugerencia(Usuario* usuario, int amigos_comunes)
{
    this->usuario = usuario;
    this->amigos_comunes = amigos_comunes;
}

Sugerencia::Sugerencia(const Sugerencia& otra)
{
    this->usuario = otra.usuario;
    this->amigos_comunes = otra.amigos_comunes;
}

const Sugerencia& Sugerencia::operator=(const Sugerencia& otra)
{
    if (this != &otra)
    {
        this->usuario = otra.usuario;
        this->amigos_comunes = otra.amigos_comunes;
    }

    return *this;
}

bool Sugerencia::operator==(const Sugerencia& otra) const
{
    return this->usuario == otra.usuario &&
           this->amigos_comunes == otra.amigos_comunes;
}

Usuario* Sugerencia::get_usuario() const
{
    return this->usuario;
}

int Sugerencia::get_amigos_comunes() const
{
    return this->amigos_comunes;
}

void Sugerencia::set_usuario(Usuario* usuario)
{
    this->usuario = usuario;
}

void Sugerencia::set_amigos_comunes(int amigos_comunes)
{
    this->amigos_comunes = amigos_comunes;
}
