
#include "album.h"
#include <iostream>

Album::Album()
    : nombre(""), ruta(""), privacidad("privado"),
    propietario(nullptr), fotos(nullptr),
    cantidad_fotos(0), capacidad_fotos(0)
{
}

Album::Album(string nombre, Usuario* propietario,
             string ruta, string privacidad)
    : nombre(nombre), ruta(ruta), privacidad("privado"),
    propietario(propietario), fotos(nullptr),
    cantidad_fotos(0), capacidad_fotos(0)
{
    this->set_privacidad(privacidad);
}

Album::Album(const Album& otro)
    : nombre(otro.nombre), ruta(otro.ruta),
    privacidad(otro.privacidad),
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
        this->ruta = otro.ruta;
        this->privacidad = otro.privacidad;
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

void Album::set_ruta(string ruta)
{
    this->ruta = ruta;
}

bool Album::set_privacidad(string privacidad)
{
    if (privacidad != "publico" &&
        privacidad != "amigos" &&
        privacidad != "privado")
        return false;

    this->privacidad = privacidad;

    return true;
}

void Album::set_propietario(Usuario* propietario)
{
    this->propietario = propietario;
}

string Album::get_nombre() const
{
    return this->nombre;
}

string Album::get_ruta() const
{
    return this->ruta;
}

string Album::get_privacidad() const
{
    return this->privacidad;
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

Foto* Album::get_foto(int posicion)
{
    if (posicion < 0 || posicion >= this->cantidad_fotos)
        return nullptr;

    return &this->fotos[posicion];
}

const Foto* Album::get_foto(int posicion) const
{
    if (posicion < 0 || posicion >= this->cantidad_fotos)
        return nullptr;

    return &this->fotos[posicion];
}

bool Album::puede_ver(Usuario* visitante, bool es_amigo) const
{
    if (visitante == nullptr)
        return false;

    if (visitante == this->propietario)
        return true;

    if (this->privacidad == "publico")
        return true;

    return this->privacidad == "amigos" && es_amigo;
}

void Album::ver_carrusel(Usuario* visitante, bool es_amigo)
{
    if (!this->puede_ver(visitante, es_amigo)) {
        cout << "No tienes permiso para ver este album.\n";
        return;
    }

    if (this->cantidad_fotos == 0) {
        cout << "El album no tiene fotos.\n";
        return;
    }

    bool* dio_like = new bool[this->cantidad_fotos]();

    int actual = 0;
    string opcion;

    while (true) {

        Foto& foto = this->fotos[actual];

        cout << "\nAlbum: " << this->nombre << "\n";
        cout << "Foto " << actual + 1
             << " de " << this->cantidad_fotos << "\n";

        cout << "Ruta: " << foto.get_ruta() << "\n";
        cout << "Likes: " << foto.get_likes() << "\n";

        cout << "1. Siguiente\n";
        cout << "2. Anterior\n";
        cout << "3. Dar/quitar like\n";
        cout << "0. Salir\n";
        cout << "Opcion: ";

        if (!getline(cin, opcion))
            break;

        if (opcion == "0")
            break;

        if (opcion == "1") {
            actual = (actual + 1) % this->cantidad_fotos;
        }
        else if (opcion == "2") {
            actual = (actual + this->cantidad_fotos - 1)
            % this->cantidad_fotos;
        }
        else if (opcion == "3") {

            if (dio_like[actual]) {
                if (foto.quitar_like())
                    dio_like[actual] = false;
            }
            else {
                foto.dar_like();
                dio_like[actual] = true;
            }
        }
        else {
            cout << "Opcion invalida.\n";
        }
    }

    delete[] dio_like;
}
