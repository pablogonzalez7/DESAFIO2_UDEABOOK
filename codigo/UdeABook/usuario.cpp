#include "usuario.h"

Usuario::Usuario()
    : solicitudes_recibidas(nullptr),
    cantidad_solicitudes(0),
    capacidad_solicitudes(0)
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
    capacidad_solicitudes(0)
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
}

Usuario::~Usuario()
{
    delete[] this->solicitudes_recibidas;
}

const Usuario& Usuario::operator=(const Usuario& otro)
{
    if (this != &otro) {
        this->nombre = otro.nombre;
        this->contra = otro.contra;
        this->correo = otro.correo;
        this->ciudad = otro.ciudad;
        this->pais = otro.pais;
        this->nacimiento = otro.nacimiento;
        this->inscripcion = otro.inscripcion;

        Usuario** nuevas_solicitudes = nullptr;

        if (otro.capacidad_solicitudes > 0) {
            nuevas_solicitudes =
                new Usuario*[otro.capacidad_solicitudes]();

            for (int i = 0; i < otro.cantidad_solicitudes; i++) {
                nuevas_solicitudes[i] =
                    otro.solicitudes_recibidas[i];
            }
        }

        delete[] this->solicitudes_recibidas;

        this->solicitudes_recibidas = nuevas_solicitudes;
        this->cantidad_solicitudes = otro.cantidad_solicitudes;
        this->capacidad_solicitudes = otro.capacidad_solicitudes;
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
