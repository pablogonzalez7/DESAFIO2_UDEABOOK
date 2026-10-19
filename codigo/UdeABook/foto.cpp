#include "recursos.h"
#include "foto.h"

Foto::Foto()
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(this)));

    this->ruta = "";
    this->descripcion = "";
    this->tamano = 0;
    this->likes = 0;
}

Foto::Foto(const string &ruta, const string &descripcion, Fecha fecha_captura, unsigned int tamano, unsigned int likes)
{
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(fecha_captura)) + static_cast<unsigned int>(sizeof(tamano)) + static_cast<unsigned int>(sizeof(likes)) + static_cast<unsigned int>(sizeof(this)));

    this->set_datos(ruta, descripcion, fecha_captura, tamano, likes);
}

Foto::Foto(const Foto &otra)
{
    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(this)));

    this->ruta = otra.ruta;
    this->descripcion = otra.descripcion;
    this->fecha_captura = otra.fecha_captura;
    this->tamano = otra.tamano;
    this->likes = otra.likes;
}

Foto::~Foto()
{
    RegistroLocal memoria_4(static_cast<unsigned int>(sizeof(this)));
}

const Foto &Foto::operator=(const Foto &otra)
{
    RegistroLocal memoria_5(static_cast<unsigned int>(sizeof(this)));

    if (this != &otra)
    {
        this->ruta = otra.ruta;
        this->descripcion = otra.descripcion;
        this->fecha_captura = otra.fecha_captura;
        this->tamano = otra.tamano;
        this->likes = otra.likes;
    }

    return *this;
}

bool Foto::operator==(const Foto &otra) const
{
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(this)));

    return this->ruta == otra.ruta;
}

void Foto::set_datos(const string &ruta, const string &descripcion, Fecha fecha_captura, unsigned int tamano,
                     unsigned int likes)
{
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(fecha_captura)) + static_cast<unsigned int>(sizeof(tamano)) + static_cast<unsigned int>(sizeof(likes)) + static_cast<unsigned int>(sizeof(this)));

    this->ruta = ruta;
    this->descripcion = descripcion;
    this->fecha_captura = fecha_captura;
    this->tamano = tamano;
    this->likes = likes;
}

bool Foto::set_tamano(unsigned int tamano)
{
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(tamano)) + static_cast<unsigned int>(sizeof(this)));

    this->tamano = tamano;

    return true;
}

bool Foto::set_likes(unsigned int likes)
{
    RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(likes)) + static_cast<unsigned int>(sizeof(this)));

    this->likes = likes;

    return true;
}

const string &Foto::get_ruta() const
{
    RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(this)));

    return this->ruta;
}

const string &Foto::get_descripcion() const
{
    RegistroLocal memoria_11(static_cast<unsigned int>(sizeof(this)));

    return this->descripcion;
}

Fecha Foto::get_fecha_captura() const
{
    RegistroLocal memoria_12(static_cast<unsigned int>(sizeof(this)));

    return this->fecha_captura;
}

Fecha Foto::get_fecha_publicacion() const
{
    RegistroLocal memoria_13(static_cast<unsigned int>(sizeof(this)));

    return this->fecha_captura;
}

unsigned int Foto::get_tamano() const
{
    RegistroLocal memoria_14(static_cast<unsigned int>(sizeof(this)));

    return this->tamano;
}

unsigned int Foto::get_likes() const
{
    RegistroLocal memoria_15(static_cast<unsigned int>(sizeof(this)));

    return this->likes;
}

void Foto::dar_like()
{
    RegistroLocal memoria_16(static_cast<unsigned int>(sizeof(this)));

    if (this->likes != static_cast<unsigned int>(-1))
        this->likes++;
}

bool Foto::quitar_like()
{
    RegistroLocal memoria_17(static_cast<unsigned int>(sizeof(this)));

    if (this->likes == 0)
        return false;

    this->likes--;

    return true;
}
