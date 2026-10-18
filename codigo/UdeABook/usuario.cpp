
#include "usuario.h"

Usuario::Usuario()
    : solicitudes_recibidas(nullptr),
    cantidad_solicitudes(0),
    capacidad_solicitudes(0),
    albumes(nullptr),
    cantidad_albumes(0),
    capacidad_albumes(0)
{
}

Usuario::Usuario(string nombre,
                 string correo,
                 string contra,
                 string ciudad,
                 string pais,
                 Fecha nacimiento,
                 Fecha inscripcion)
    : nombre(nombre),
    contra(contra),
    correo(correo),
    ciudad(ciudad),
    pais(pais),
    nacimiento(nacimiento),
    inscripcion(inscripcion),
    solicitudes_recibidas(nullptr),
    cantidad_solicitudes(0),
    capacidad_solicitudes(0),
    albumes(nullptr),
    cantidad_albumes(0),
    capacidad_albumes(0)
{
}

Usuario::Usuario(const Usuario& otro)
{
    this->nombre = otro.nombre;
    this->contra = otro.contra;
    this->correo = otro.correo;
    this->ciudad = otro.ciudad;
    this->pais = otro.pais;
    this->nacimiento = otro.nacimiento;
    this->inscripcion = otro.inscripcion;

    this->solicitudes_recibidas = nullptr;
    this->cantidad_solicitudes = otro.cantidad_solicitudes;
    this->capacidad_solicitudes = otro.capacidad_solicitudes;

    if (this->capacidad_solicitudes > 0) {

        this->solicitudes_recibidas =
            new Usuario*[this->capacidad_solicitudes]();

        for (int i = 0; i < this->cantidad_solicitudes; i++) {
            this->solicitudes_recibidas[i] =
                otro.solicitudes_recibidas[i];
        }
    }

    this->albumes = nullptr;
    this->cantidad_albumes = otro.cantidad_albumes;
    this->capacidad_albumes = otro.capacidad_albumes;

    if (this->capacidad_albumes > 0) {

        this->albumes = new Album[this->capacidad_albumes];

        for (int i = 0; i < this->cantidad_albumes; i++) {
            this->albumes[i] = otro.albumes[i];
            this->albumes[i].set_propietario(this);
        }
    }
}

Usuario::~Usuario()
{
    delete[] this->solicitudes_recibidas;
    delete[] this->albumes;
}

const Usuario& Usuario::operator=(const Usuario& otro)
{
    if (this != &otro) {

        Usuario** nuevas_solicitudes = nullptr;

        if (otro.capacidad_solicitudes > 0) {

            nuevas_solicitudes =
                new Usuario*[otro.capacidad_solicitudes]();

            for (int i = 0; i < otro.cantidad_solicitudes; i++) {
                nuevas_solicitudes[i] =
                    otro.solicitudes_recibidas[i];
            }
        }

        Album* nuevos_albumes = nullptr;

        if (otro.capacidad_albumes > 0) {

            nuevos_albumes = new Album[otro.capacidad_albumes];

            for (int i = 0; i < otro.cantidad_albumes; i++) {
                nuevos_albumes[i] = otro.albumes[i];
                nuevos_albumes[i].set_propietario(this);
            }
        }

        delete[] this->solicitudes_recibidas;
        delete[] this->albumes;

        this->nombre = otro.nombre;
        this->contra = otro.contra;
        this->correo = otro.correo;
        this->ciudad = otro.ciudad;
        this->pais = otro.pais;
        this->nacimiento = otro.nacimiento;
        this->inscripcion = otro.inscripcion;

        this->solicitudes_recibidas = nuevas_solicitudes;
        this->cantidad_solicitudes = otro.cantidad_solicitudes;
        this->capacidad_solicitudes = otro.capacidad_solicitudes;

        this->albumes = nuevos_albumes;
        this->cantidad_albumes = otro.cantidad_albumes;
        this->capacidad_albumes = otro.capacidad_albumes;
    }

    return *this;
}

bool Usuario::operator==(const Usuario& otro) const
{
    return this->correo == otro.correo;
}

void Usuario::set_datos(string nombre,
                        string correo,
                        string contra,
                        string ciudad,
                        string pais,
                        Fecha nacimiento,
                        Fecha inscripcion)
{
    this->nombre = nombre;
    this->correo = correo;
    this->contra = contra;
    this->ciudad = ciudad;
    this->pais = pais;
    this->nacimiento = nacimiento;
    this->inscripcion = inscripcion;
}

string Usuario::get_nombre() const
{
    return this->nombre;
}

string Usuario::get_correo() const
{
    return this->correo;
}

string Usuario::get_ciudad() const
{
    return this->ciudad;
}

string Usuario::get_pais() const
{
    return this->pais;
}

Fecha Usuario::get_nacimiento() const
{
    return this->nacimiento;
}

Fecha Usuario::get_inscripcion() const
{
    return this->inscripcion;
}

bool Usuario::verificar_contra(const string& intento) const
{
    return this->contra == intento;
}

int Usuario::get_cantidad_albumes() const
{
    return this->cantidad_albumes;
}

bool Usuario::agregar_album(const Album& album)
{
    if (album.get_nombre().empty())
        return false;

    for (int i = 0; i < this->cantidad_albumes; i++) {

        if (this->albumes[i].get_nombre() == album.get_nombre())
            return false;
    }

    if (this->cantidad_albumes == this->capacidad_albumes) {

        int nueva_capacidad = this->capacidad_albumes == 0
                                  ? 2 : this->capacidad_albumes * 2;

        Album* nuevos_albumes = new Album[nueva_capacidad];

        for (int i = 0; i < this->cantidad_albumes; i++) {
            nuevos_albumes[i] = this->albumes[i];
            nuevos_albumes[i].set_propietario(this);
        }

        delete[] this->albumes;

        this->albumes = nuevos_albumes;
        this->capacidad_albumes = nueva_capacidad;
    }

    this->albumes[this->cantidad_albumes] = album;
    this->albumes[this->cantidad_albumes].set_propietario(this);

    this->cantidad_albumes++;

    return true;
}

Album* Usuario::get_album(int posicion)
{
    if (posicion < 0 || posicion >= this->cantidad_albumes)
        return nullptr;

    return &this->albumes[posicion];
}

const Album* Usuario::get_album(int posicion) const
{
    if (posicion < 0 || posicion >= this->cantidad_albumes)
        return nullptr;

    return &this->albumes[posicion];
}
