
#include "album.h"

Album::Album()
    : nombre(""),
    propietario(nullptr),
    fotos(nullptr),
    cantidad_fotos(0),
    capacidad_fotos(0)
{
}

Album::Album(string nombre, Usuario* propietario)
    : nombre(nombre),
    propietario(propietario),
    fotos(nullptr),
    cantidad_fotos(0),
    capacidad_fotos(0)
{
}

Album::Album(const Album& otro)
    : nombre(otro.nombre),
    propietario(otro.propietario),
    fotos(nullptr),
    cantidad_fotos(otro.cantidad_fotos),
    capacidad_fotos(otro.capacidad_fotos)
{
    if (this->capacidad_fotos > 0) {
        this->fotos = new Foto[this->capacidad_fotos];

        for (int i = 0; i < this->cantidad_fotos; i++) {
            this->fotos[i] = otro.fotos[i];
        }
    }
}

Album::~Album()
{
    delete[] this->fotos;
}

const Album& Album::operator=(const Album& otro)
{
    if (this != &otro) {

        Foto* nuevas_fotos = nullptr;

        if (otro.capacidad_fotos > 0) {
            nuevas_fotos = new Foto[otro.capacidad_fotos];

            for (int i = 0; i < otro.cantidad_fotos; i++) {
                nuevas_fotos[i] = otro.fotos[i];
            }
        }

        delete[] this->fotos;

        this->nombre = otro.nombre;
        this->propietario = otro.propietario;
        this->fotos = nuevas_fotos;
        this->cantidad_fotos = otro.cantidad_fotos;
        this->capacidad_fotos = otro.capacidad_fotos;
    }

    return *this;
}

bool Album::operator==(const Album& otro) const
{
    return this->nombre == otro.nombre &&
           this->propietario == otro.propietario;
}

void Album::set_nombre(string nombre)
{
    this->nombre = nombre;
}

string Album::get_nombre() const
{
    return this->nombre;
}

void Album::set_propietario(Usuario* propietario)
{
    this->propietario = propietario;
}

Usuario* Album::get_propietario() const
{
    return this->propietario;
}

int Album::get_cantidad_fotos() const
{
    return this->cantidad_fotos;
}

bool Album::agregar_foto(const Foto& foto)
{
    if (foto.get_ruta().empty())
        return false;

    if (this->cantidad_fotos == this->capacidad_fotos) {

        int nueva_capacidad = this->capacidad_fotos == 0
                                  ? 2 : this->capacidad_fotos * 2;

        Foto* nuevo_arreglo = new Foto[nueva_capacidad];

        for (int i = 0; i < this->cantidad_fotos; i++) {
            nuevo_arreglo[i] = this->fotos[i];
        }

        delete[] this->fotos;

        this->fotos = nuevo_arreglo;
        this->capacidad_fotos = nueva_capacidad;
    }

    this->fotos[this->cantidad_fotos] = foto;
    this->cantidad_fotos++;

    return true;
}

bool Album::eliminar_foto(int posicion)
{
    if (posicion < 0 || posicion >= this->cantidad_fotos)
        return false;

    for (int i = posicion; i < this->cantidad_fotos - 1; i++) {
        this->fotos[i] = this->fotos[i + 1];
    }

    this->cantidad_fotos--;
    this->fotos[this->cantidad_fotos] = Foto();

    return true;
}

const Foto* Album::get_foto(int posicion) const
{
    if (posicion < 0 || posicion >= this->cantidad_fotos)
        return nullptr;

    return &this->fotos[posicion];
}
