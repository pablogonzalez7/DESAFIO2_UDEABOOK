#include "recursos.h"
#include "sugerencia.h"

Sugerencia::Sugerencia()
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(this)));

    this->usuario = nullptr;
    this->amigos_comunes = 0;
}

Sugerencia::Sugerencia(Usuario *usuario, unsigned int amigos_comunes)
{
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(usuario)) + static_cast<unsigned int>(sizeof(amigos_comunes)) + static_cast<unsigned int>(sizeof(this)));

    this->usuario = usuario;
    this->amigos_comunes = amigos_comunes;
}

Sugerencia::Sugerencia(const Sugerencia &otra)
{
    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(this)));

    this->usuario = otra.usuario;
    this->amigos_comunes = otra.amigos_comunes;
}

const Sugerencia &Sugerencia::operator=(const Sugerencia &otra)
{
    RegistroLocal memoria_4(static_cast<unsigned int>(sizeof(this)));

    if (this != &otra)
    {
        this->usuario = otra.usuario;
        this->amigos_comunes = otra.amigos_comunes;
    }

    return *this;
}

bool Sugerencia::operator==(const Sugerencia &otra) const
{
    RegistroLocal memoria_5(static_cast<unsigned int>(sizeof(this)));

    return this->usuario == otra.usuario && this->amigos_comunes == otra.amigos_comunes;
}

Usuario *Sugerencia::get_usuario() const
{
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(this)));

    return this->usuario;
}

unsigned int Sugerencia::get_amigos_comunes() const
{
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(this)));

    return this->amigos_comunes;
}

void Sugerencia::set_usuario(Usuario *usuario)
{
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(usuario)) + static_cast<unsigned int>(sizeof(this)));

    this->usuario = usuario;
}

void Sugerencia::set_amigos_comunes(unsigned int amigos_comunes)
{
    RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(amigos_comunes)) + static_cast<unsigned int>(sizeof(this)));

    this->amigos_comunes = amigos_comunes;
}
