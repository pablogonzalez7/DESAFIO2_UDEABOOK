#include "recursos.h"
#include "mensaje.h"

Mensaje::Mensaje()
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(this)));

    this->contenido = "";
    this->emisor = nullptr;
    this->receptor = nullptr;
    this->leido = false;
}

Mensaje::Mensaje(const string &contenido, Usuario *emisor, Usuario *receptor, const Fecha &fecha, bool leido)
{
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(emisor)) + static_cast<unsigned int>(sizeof(receptor)) + static_cast<unsigned int>(sizeof(leido)) + static_cast<unsigned int>(sizeof(this)));

    this->contenido = contenido;
    this->emisor = emisor;
    this->receptor = receptor;
    this->fecha = fecha;
    this->leido = leido;
}

Mensaje::Mensaje(const Mensaje &otro)
{
    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(this)));

    this->contenido = otro.contenido;
    this->emisor = otro.emisor;
    this->receptor = otro.receptor;
    this->fecha = otro.fecha;
    this->leido = otro.leido;
}

const Mensaje &Mensaje::operator=(const Mensaje &otro)
{
    RegistroLocal memoria_4(static_cast<unsigned int>(sizeof(this)));

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

bool Mensaje::operator==(const Mensaje &otro) const
{
    RegistroLocal memoria_5(static_cast<unsigned int>(sizeof(this)));

    return this->contenido == otro.contenido && this->emisor == otro.emisor &&
           this->receptor == otro.receptor && this->fecha.get_dia() == otro.fecha.get_dia() &&
           this->fecha.get_mes() == otro.fecha.get_mes() && this->fecha.get_anio() == otro.fecha.get_anio() &&
           this->leido == otro.leido;
}

const string &Mensaje::get_contenido() const
{
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(this)));

    return this->contenido;
}

Usuario *Mensaje::get_emisor() const
{
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(this)));

    return this->emisor;
}

Usuario *Mensaje::get_receptor() const
{
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(this)));

    return this->receptor;
}

const Fecha &Mensaje::get_fecha() const
{
    RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(this)));

    return this->fecha;
}

bool Mensaje::get_leido() const
{
    RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(this)));

    return this->leido;
}

void Mensaje::set_datos(const string &contenido, Usuario *emisor, Usuario *receptor, const Fecha &fecha,
                        bool leido)
{
    RegistroLocal memoria_11(static_cast<unsigned int>(sizeof(emisor)) + static_cast<unsigned int>(sizeof(receptor)) + static_cast<unsigned int>(sizeof(leido)) + static_cast<unsigned int>(sizeof(this)));

    this->contenido = contenido;
    this->emisor = emisor;
    this->receptor = receptor;
    this->fecha = fecha;
    this->leido = leido;
}

void Mensaje::marcar_leido()
{
    RegistroLocal memoria_12(static_cast<unsigned int>(sizeof(this)));

    this->leido = true;
}

void Mensaje::intercambiar(Mensaje &otro) noexcept
{
    RegistroLocal memoria_13(static_cast<unsigned int>(sizeof(this)));

    string temporal = static_cast<string &&>(this->contenido);
    RegistroLocal memoria_14(static_cast<unsigned int>(sizeof(temporal)));

    this->contenido = static_cast<string &&>(otro.contenido);
    otro.contenido = static_cast<string &&>(temporal);
    Usuario *usuario = this->emisor;
    RegistroLocal memoria_15(static_cast<unsigned int>(sizeof(usuario)));

    this->emisor = otro.emisor;
    otro.emisor = usuario;
    usuario = this->receptor;
    this->receptor = otro.receptor;
    otro.receptor = usuario;
    Fecha fecha = this->fecha;
    RegistroLocal memoria_16(static_cast<unsigned int>(sizeof(fecha)));

    this->fecha = otro.fecha;
    otro.fecha = fecha;
    bool leido = this->leido;
    RegistroLocal memoria_17(static_cast<unsigned int>(sizeof(leido)));

    this->leido = otro.leido;
    otro.leido = leido;
}
