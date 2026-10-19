#include "recursos.h"
#include "album.h"
#include <iostream>
#include "archivo.h"

Album::Album()
    : nombre(""), ruta(""), privacidad(2), propietario(nullptr), fotos(nullptr), cantidad_fotos(0),
    capacidad_fotos(0)
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(this)));
}

Album::Album(const string &nombre, Usuario *propietario, const string &ruta, const string &privacidad)
    : nombre(nombre), ruta(ruta), privacidad(2), propietario(propietario), fotos(nullptr), cantidad_fotos(0),
    capacidad_fotos(0)
{
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(propietario)) + static_cast<unsigned int>(sizeof(this)));

    this->set_privacidad(privacidad);
}

Album::Album(const Album &otro)
{
    this->privacidad = 2;
    this->propietario = nullptr;
    this->fotos = nullptr;
    this->cantidad_fotos = 0;
    this->capacidad_fotos = 0;

    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(this)));

    try
    {
        this->nombre = otro.nombre;
        this->ruta = otro.ruta;
        this->privacidad = otro.privacidad;
        this->propietario = otro.propietario;
        if (otro.capacidad_fotos != 0)
            this->fotos = registrar_reserva(new Foto[otro.capacidad_fotos],
                                            (this->capacidad_fotos = otro.capacidad_fotos) * static_cast<unsigned int>(sizeof(Foto)));
        this->capacidad_fotos = otro.capacidad_fotos;
        {
            RegistroLocal memoria_4(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < otro.cantidad_fotos; i++)
            {
                contar_iteracion();
                this->fotos[i] = otro.fotos[i];
            }
        }
        this->cantidad_fotos = otro.cantidad_fotos;
    }
    catch (...)
    {
        (registrar_liberacion(this->fotos != nullptr ? this->capacidad_fotos * static_cast<unsigned int>(sizeof(Foto)) : 0),
         delete[] this->fotos);
        this->fotos = nullptr;
        throw;
    }
}

Album::~Album()
{
    RegistroLocal memoria_5(static_cast<unsigned int>(sizeof(this)));

    (registrar_liberacion(this->fotos != nullptr ? this->capacidad_fotos * static_cast<unsigned int>(sizeof(Foto)) : 0),
     delete[] this->fotos);
}

const Album &Album::operator=(const Album &otro)
{
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(this)));

    if (this == &otro)
        return *this;
    Album copia(otro);
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(copia)));

    this->nombre = copia.nombre;
    this->ruta = copia.ruta;
    this->privacidad = copia.privacidad;
    this->propietario = copia.propietario;
    (registrar_liberacion(this->fotos != nullptr ? this->capacidad_fotos * static_cast<unsigned int>(sizeof(Foto)) : 0),
     delete[] this->fotos);
    this->fotos = copia.fotos;
    copia.fotos = nullptr;
    this->cantidad_fotos = copia.cantidad_fotos;
    this->capacidad_fotos = copia.capacidad_fotos;
    return *this;
}

bool Album::operator==(const Album &otro) const
{
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(this)));

    return this->nombre == otro.nombre && this->propietario == otro.propietario;
}

void Album::set_nombre(const string &nombre)
{
    RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(this)));

    this->nombre = nombre;
}

void Album::set_ruta(const string &ruta)
{
    RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(this)));

    this->ruta = ruta;
}

bool Album::set_privacidad(const string &privacidad)
{
    RegistroLocal memoria_11(static_cast<unsigned int>(sizeof(this)));

    if (privacidad == "publico")
        this->privacidad = 0;
    else if (privacidad == "amigos")
        this->privacidad = 1;
    else if (privacidad == "privado")
        this->privacidad = 2;
    else
        return false;
    return true;
}

void Album::set_propietario(Usuario *propietario)
{
    RegistroLocal memoria_12(static_cast<unsigned int>(sizeof(propietario)) + static_cast<unsigned int>(sizeof(this)));

    this->propietario = propietario;
}

const string &Album::get_nombre() const
{
    RegistroLocal memoria_13(static_cast<unsigned int>(sizeof(this)));

    return this->nombre;
}

const string &Album::get_ruta() const
{
    RegistroLocal memoria_14(static_cast<unsigned int>(sizeof(this)));

    return this->ruta;
}

string Album::get_privacidad() const
{
    RegistroLocal memoria_15(static_cast<unsigned int>(sizeof(this)));

    if (this->privacidad == 0)
        return "publico";
    if (this->privacidad == 1)
        return "amigos";
    return "privado";
}

Usuario *Album::get_propietario() const
{
    RegistroLocal memoria_16(static_cast<unsigned int>(sizeof(this)));

    return this->propietario;
}

unsigned int Album::get_cantidad_fotos() const
{
    RegistroLocal memoria_17(static_cast<unsigned int>(sizeof(this)));

    return this->cantidad_fotos;
}

bool Album::reservar_fotos(unsigned int cantidad)
{
    RegistroLocal memoria_18(static_cast<unsigned int>(sizeof(cantidad)) + static_cast<unsigned int>(sizeof(this)));

    if (this->fotos != nullptr || cantidad > static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(Foto)))
        return false;
    try
    {
        if (cantidad != 0)
            this->fotos =
                registrar_reserva(new Foto[cantidad], (this->capacidad_fotos = cantidad) * static_cast<unsigned int>(sizeof(Foto)));
        this->capacidad_fotos = cantidad;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo reservar memoria para las fotos.\n";
        }
        return false;
    }
}
bool Album::agregar_foto(const Foto &foto)
{
    RegistroLocal memoria_19(static_cast<unsigned int>(sizeof(this)));

    if (foto.get_ruta() == "" || this->cantidad_fotos == this->capacidad_fotos)
        return false;
    try
    {
        this->fotos[this->cantidad_fotos] = foto;
        this->cantidad_fotos++;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo almacenar la foto.\n";
        }
        return false;
    }
}

bool Album::eliminar_foto(unsigned int posicion)
{
    RegistroLocal memoria_20(static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_fotos)
        return false;

    {
        RegistroLocal memoria_21(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = posicion; i < this->cantidad_fotos - 1; i++)
        {
            contar_iteracion();
            this->fotos[i] = this->fotos[i + 1];
        }
    }

    this->cantidad_fotos--;
    this->fotos[this->cantidad_fotos] = Foto();

    return true;
}

Foto *Album::get_foto(unsigned int posicion)
{
    RegistroLocal memoria_22(static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_fotos)
        return nullptr;

    return &this->fotos[posicion];
}

const Foto *Album::get_foto(unsigned int posicion) const
{
    RegistroLocal memoria_23(static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_fotos)
        return nullptr;

    return &this->fotos[posicion];
}

bool Album::puede_ver(Usuario *visitante, bool es_amigo) const
{
    RegistroLocal memoria_24(static_cast<unsigned int>(sizeof(visitante)) + static_cast<unsigned int>(sizeof(es_amigo)) + static_cast<unsigned int>(sizeof(this)));

    if (visitante == nullptr)
        return false;

    if (visitante == this->propietario)
        return true;

    if (this->privacidad == 0)
        return true;

    return this->privacidad == 1 && es_amigo;
}

void Album::ver_carrusel(Usuario *visitante, bool es_amigo)
{
    RegistroLocal memoria_25(static_cast<unsigned int>(sizeof(visitante)) + static_cast<unsigned int>(sizeof(es_amigo)) + static_cast<unsigned int>(sizeof(this)));

    if (!this->puede_ver(visitante, es_amigo))
    {
        {
            contar_llamadas(9, 1);
            cout << "No tienes permiso para ver este album.\n";
        }
        return;
    }

    if (this->cantidad_fotos == 0)
    {
        {
            contar_llamadas(9, 1);
            cout << "El album no tiene fotos.\n";
        }
        return;
    }

    unsigned int actual = 0;
    RegistroLocal memoria_26(static_cast<unsigned int>(sizeof(actual)));

    string opcion;
    RegistroLocal memoria_27(static_cast<unsigned int>(sizeof(opcion)));

    while (true)
    {
        contar_iteracion();

        Foto &foto = this->fotos[actual];

        {
            contar_llamadas(9, 3);
            cout << "\nAlbum: " << this->nombre << "\n";
        }
        {
            contar_llamadas(9, 5);
            cout << "Foto " << actual + 1 << " de " << this->cantidad_fotos << "\n";
        }

        {
            contar_llamadas(9, 3);
            cout << "Ruta: " << foto.get_ruta() << "\n";
        }
        {
            contar_llamadas(9, 3);
            cout << "Likes: " << foto.get_likes() << "\n";
        }

        {
            contar_llamadas(9, 1);
            cout << "1. Siguiente\n";
        }
        {
            contar_llamadas(9, 1);
            cout << "2. Anterior\n";
        }
        {
            contar_llamadas(9, 1);
            cout << "3. Dar like\n";
        }
        {
            contar_llamadas(9, 1);
            cout << "4. Quitar like\n";
        }
        {
            contar_llamadas(9, 1);
            cout << "0. Salir\n";
        }
        {
            contar_llamadas(9, 1);
            cout << "Opcion: ";
        }

        if (!(contar_llamada(0), getline(cin, opcion)))
            break;

        if (opcion == "0")
            break;

        if (opcion == "1")
        {
            actual = (actual + 1) % this->cantidad_fotos;
        }
        else if (opcion == "2")
        {
            actual = (actual + this->cantidad_fotos - 1) % this->cantidad_fotos;
        }
        else if (opcion == "3")
        {
            foto.dar_like();
        }
        else if (opcion == "4")
        {
            foto.quitar_like();
        }
        else
        {
            {
                contar_llamadas(9, 1);
                cout << "Opcion invalida.\n";
            }
        }
    }
}
