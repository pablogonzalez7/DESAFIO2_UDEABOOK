#include "recursos.h"
#include "usuario.h"
#include <iostream>

Usuario::Usuario()
    : solicitudes_recibidas(nullptr), bytes_indice(1), cantidad_solicitudes(0), capacidad_solicitudes(0),
    albumes(nullptr), cantidad_albumes(0), capacidad_albumes(0)
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(this)));
}

Usuario::Usuario(const string &nombre, const string &correo, const string &contra, const string &ciudad,
                 const string &pais, Fecha nacimiento, Fecha inscripcion)
    : nombre(nombre), contra(contra), correo(correo), ciudad(ciudad), pais(pais), nacimiento(nacimiento),
    inscripcion(inscripcion), solicitudes_recibidas(nullptr), bytes_indice(1), cantidad_solicitudes(0),
    capacidad_solicitudes(0), albumes(nullptr), cantidad_albumes(0), capacidad_albumes(0)
{
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(nacimiento)) + static_cast<unsigned int>(sizeof(inscripcion)) + static_cast<unsigned int>(sizeof(this)));
}

Usuario::Usuario(const Usuario &otro)
{
    this->solicitudes_recibidas = nullptr;
    this->bytes_indice = 1;
    this->cantidad_solicitudes = 0;
    this->capacidad_solicitudes = 0;
    this->albumes = nullptr;
    this->cantidad_albumes = 0;
    this->capacidad_albumes = 0;

    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(this)));

    try
    {
        this->nombre = otro.nombre;
        this->contra = otro.contra;
        this->correo = otro.correo;
        this->ciudad = otro.ciudad;
        this->pais = otro.pais;
        this->ruta_perfil = otro.ruta_perfil;
        this->nacimiento = otro.nacimiento;
        this->inscripcion = otro.inscripcion;
        if (otro.capacidad_solicitudes != 0)
            this->solicitudes_recibidas =
                registrar_reserva(new unsigned char[otro.capacidad_solicitudes * otro.bytes_indice](),
                                  (this->capacidad_solicitudes = otro.capacidad_solicitudes) *
                                      (this->bytes_indice = otro.bytes_indice));
        this->capacidad_solicitudes = otro.capacidad_solicitudes;
        this->bytes_indice = otro.bytes_indice;
        {
            RegistroLocal memoria_4(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < otro.cantidad_solicitudes * otro.bytes_indice; i++)
            {
                contar_iteracion();
                this->solicitudes_recibidas[i] = otro.solicitudes_recibidas[i];
            }
        }
        this->cantidad_solicitudes = otro.cantidad_solicitudes;
        if (otro.capacidad_albumes != 0)
            this->albumes =
                registrar_reserva(new Album[otro.capacidad_albumes],
                                  (this->capacidad_albumes = otro.capacidad_albumes) * static_cast<unsigned int>(sizeof(Album)));
        this->capacidad_albumes = otro.capacidad_albumes;
        {
            RegistroLocal memoria_5(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < otro.cantidad_albumes; i++)
            {
                contar_iteracion();
                this->albumes[i] = otro.albumes[i];
                this->albumes[i].set_propietario(this);
            }
        }
        this->cantidad_albumes = otro.cantidad_albumes;
    }
    catch (...)
    {
        (registrar_liberacion(
             this->solicitudes_recibidas != nullptr ? this->capacidad_solicitudes * this->bytes_indice : 0),
         delete[] this->solicitudes_recibidas);
        (registrar_liberacion(this->albumes != nullptr ? this->capacidad_albumes * static_cast<unsigned int>(sizeof(Album)) : 0),
         delete[] this->albumes);
        this->solicitudes_recibidas = nullptr;
        this->albumes = nullptr;
        throw;
    }
}

Usuario::~Usuario()
{
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(this)));

    (registrar_liberacion(
         this->solicitudes_recibidas != nullptr ? this->capacidad_solicitudes * this->bytes_indice : 0),
     delete[] this->solicitudes_recibidas);
    (registrar_liberacion(this->albumes != nullptr ? this->capacidad_albumes * static_cast<unsigned int>(sizeof(Album)) : 0),
     delete[] this->albumes);
}

const Usuario &Usuario::operator=(const Usuario &otro)
{
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(this)));

    if (this == &otro)
        return *this;
    Usuario copia(otro);
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(copia)));

    this->nombre = copia.nombre;
    this->contra = copia.contra;
    this->correo = copia.correo;
    this->ciudad = copia.ciudad;
    this->pais = copia.pais;
    this->ruta_perfil = copia.ruta_perfil;
    this->nacimiento = copia.nacimiento;
    this->inscripcion = copia.inscripcion;
    (registrar_liberacion(
         this->solicitudes_recibidas != nullptr ? this->capacidad_solicitudes * this->bytes_indice : 0),
     delete[] this->solicitudes_recibidas);
    (registrar_liberacion(this->albumes != nullptr ? this->capacidad_albumes * static_cast<unsigned int>(sizeof(Album)) : 0),
     delete[] this->albumes);
    this->solicitudes_recibidas = copia.solicitudes_recibidas;
    this->albumes = copia.albumes;
    copia.solicitudes_recibidas = nullptr;
    copia.albumes = nullptr;
    this->cantidad_solicitudes = copia.cantidad_solicitudes;
    this->capacidad_solicitudes = copia.capacidad_solicitudes;
    this->bytes_indice = copia.bytes_indice;
    this->cantidad_albumes = copia.cantidad_albumes;
    this->capacidad_albumes = copia.capacidad_albumes;
    {
        RegistroLocal memoria_9(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < this->cantidad_albumes; i++)
        {
            contar_iteracion();
            this->albumes[i].set_propietario(this);
        }
    }
    return *this;
}

bool Usuario::operator==(const Usuario &otro) const
{
    RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(this)));

    return this->nombre == otro.nombre;
}

void Usuario::set_datos(const string &nombre, const string &correo, const string &contra,
                        const string &ciudad, const string &pais, Fecha nacimiento, Fecha inscripcion)
{
    RegistroLocal memoria_11(static_cast<unsigned int>(sizeof(nacimiento)) + static_cast<unsigned int>(sizeof(inscripcion)) + static_cast<unsigned int>(sizeof(this)));

    this->nombre = nombre;
    this->correo = correo;
    this->contra = contra;
    this->ciudad = ciudad;
    this->pais = pais;
    this->nacimiento = nacimiento;
    this->inscripcion = inscripcion;
}

const string &Usuario::get_nombre() const
{
    RegistroLocal memoria_12(static_cast<unsigned int>(sizeof(this)));

    return this->nombre;
}

const string &Usuario::get_correo() const
{
    RegistroLocal memoria_13(static_cast<unsigned int>(sizeof(this)));

    return this->correo;
}

const string &Usuario::get_ciudad() const
{
    RegistroLocal memoria_14(static_cast<unsigned int>(sizeof(this)));

    return this->ciudad;
}

const string &Usuario::get_pais() const
{
    RegistroLocal memoria_15(static_cast<unsigned int>(sizeof(this)));

    return this->pais;
}

Fecha Usuario::get_nacimiento() const
{
    RegistroLocal memoria_16(static_cast<unsigned int>(sizeof(this)));

    return this->nacimiento;
}

Fecha Usuario::get_inscripcion() const
{
    RegistroLocal memoria_17(static_cast<unsigned int>(sizeof(this)));

    return this->inscripcion;
}

bool Usuario::verificar_contra(const string &intento) const
{
    RegistroLocal memoria_18(static_cast<unsigned int>(sizeof(this)));

    return this->contra == intento;
}

unsigned int Usuario::get_cantidad_albumes() const
{
    RegistroLocal memoria_19(static_cast<unsigned int>(sizeof(this)));

    return this->cantidad_albumes;
}

void Usuario::set_ruta_perfil(const string &ruta)
{
    RegistroLocal memoria_20(static_cast<unsigned int>(sizeof(this)));
    this->ruta_perfil = ruta;
}
const string &Usuario::get_ruta_perfil() const
{
    RegistroLocal memoria_21(static_cast<unsigned int>(sizeof(this)));
    return this->ruta_perfil;
}
const string &Usuario::get_contra() const
{
    RegistroLocal memoria_22(static_cast<unsigned int>(sizeof(this)));
    return this->contra;
}
bool Usuario::reservar_solicitudes(unsigned int capacidad, unsigned int total_usuarios)
{
    RegistroLocal memoria_23(static_cast<unsigned int>(sizeof(capacidad)) + static_cast<unsigned int>(sizeof(total_usuarios)) + static_cast<unsigned int>(sizeof(this)));

    if (solicitudes_recibidas != nullptr || total_usuarios == 0 || capacidad >= total_usuarios)
        return false;
    unsigned int maximo = total_usuarios - 1;
    RegistroLocal memoria_24(static_cast<unsigned int>(sizeof(maximo)));

    bytes_indice = 1;
    while (maximo > 255)
    {
        contar_iteracion();
        bytes_indice++;
        maximo >>= 8;
    }
    if (capacidad > static_cast<unsigned int>(-1) / bytes_indice)
        return false;
    try
    {
        if (capacidad != 0)
            solicitudes_recibidas = registrar_reserva(new unsigned char[capacidad * bytes_indice](),
                                                      (capacidad_solicitudes = capacidad) * bytes_indice);
        capacidad_solicitudes = capacidad;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo reservar memoria para solicitudes.\n";
        }
        return false;
    }
}
unsigned int Usuario::get_cantidad_solicitudes() const
{
    RegistroLocal memoria_25(static_cast<unsigned int>(sizeof(this)));
    return cantidad_solicitudes;
}
unsigned int Usuario::get_solicitud(unsigned int posicion) const
{
    RegistroLocal memoria_26(static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    if (posicion >= cantidad_solicitudes)
        return static_cast<unsigned int>(-1);
    unsigned int indice = 0;
    RegistroLocal memoria_27(static_cast<unsigned int>(sizeof(indice)));

    {
        RegistroLocal memoria_28(1 * static_cast<unsigned int>(sizeof(unsigned char)));
        for (unsigned char b = 0; b < bytes_indice; b++)
        {
            contar_iteracion();
            indice |= static_cast<unsigned int>(solicitudes_recibidas[posicion * bytes_indice + b]) << (8 * b);
        }
    }
    return indice;
}
bool Usuario::tiene_solicitud(unsigned int usuario) const
{
    RegistroLocal memoria_29(static_cast<unsigned int>(sizeof(usuario)) + static_cast<unsigned int>(sizeof(this)));

    {
        RegistroLocal memoria_30(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < cantidad_solicitudes; i++)
        {
            contar_iteracion();
            if (get_solicitud(i) == usuario)
                return true;
        }
    }
    return false;
}
bool Usuario::agregar_solicitud(unsigned int usuario)
{
    RegistroLocal memoria_31(static_cast<unsigned int>(sizeof(usuario)) + static_cast<unsigned int>(sizeof(this)));

    if (tiene_solicitud(usuario) || cantidad_solicitudes == capacidad_solicitudes)
        return false;
    unsigned int valor = usuario;
    RegistroLocal memoria_32(static_cast<unsigned int>(sizeof(valor)));

    {
        RegistroLocal memoria_33(1 * static_cast<unsigned int>(sizeof(unsigned char)));
        for (unsigned char b = 0; b < bytes_indice; b++)
        {
            contar_iteracion();
            solicitudes_recibidas[cantidad_solicitudes * bytes_indice + b] =
                static_cast<unsigned char>(valor & 255);
            valor >>= 8;
        }
    }
    cantidad_solicitudes++;
    return true;
}
bool Usuario::retirar_solicitud(unsigned int posicion)
{
    RegistroLocal memoria_34(static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    if (posicion >= cantidad_solicitudes)
        return false;
    {
        RegistroLocal memoria_35(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = posicion * bytes_indice; i + bytes_indice < cantidad_solicitudes * bytes_indice; i++)
        {
            contar_iteracion();
            solicitudes_recibidas[i] = solicitudes_recibidas[i + bytes_indice];
        }
    }
    cantidad_solicitudes--;
    return true;
}
bool Usuario::reservar_albumes(unsigned int cantidad)
{
    RegistroLocal memoria_36(static_cast<unsigned int>(sizeof(cantidad)) + static_cast<unsigned int>(sizeof(this)));

    if (this->albumes != nullptr || cantidad > static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(Album)))
        return false;
    try
    {
        if (cantidad != 0)
            this->albumes =
                registrar_reserva(new Album[cantidad], (this->capacidad_albumes = cantidad) * static_cast<unsigned int>(sizeof(Album)));
        this->capacidad_albumes = cantidad;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo reservar memoria para albumes.\n";
        }
        return false;
    }
}
bool Usuario::agregar_album(const Album &album)
{
    RegistroLocal memoria_37(static_cast<unsigned int>(sizeof(this)));

    if (album.get_nombre() == "" || this->cantidad_albumes == this->capacidad_albumes)
        return false;
    {
        RegistroLocal memoria_38(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < this->cantidad_albumes; i++)
        {
            contar_iteracion();
            if (this->albumes[i].get_nombre() == album.get_nombre())
                return false;
        }
    }
    try
    {
        this->albumes[this->cantidad_albumes] = album;
        this->albumes[this->cantidad_albumes].set_propietario(this);
        this->cantidad_albumes++;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo almacenar el album.\n";
        }
        return false;
    }
}

Album *Usuario::get_album(unsigned int posicion)
{
    RegistroLocal memoria_39(static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_albumes)
        return nullptr;

    return &this->albumes[posicion];
}

const Album *Usuario::get_album(unsigned int posicion) const
{
    RegistroLocal memoria_40(static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_albumes)
        return nullptr;

    return &this->albumes[posicion];
}

void Usuario::intercambiar(Usuario &otro) noexcept
{
    RegistroLocal memoria_41(static_cast<unsigned int>(sizeof(this)));

    {
        string temporal(static_cast<string &&>(nombre));
        nombre = static_cast<string &&>(otro.nombre);
        otro.nombre = static_cast<string &&>(temporal);
    }
    {
        string temporal(static_cast<string &&>(contra));
        contra = static_cast<string &&>(otro.contra);
        otro.contra = static_cast<string &&>(temporal);
    }
    {
        string temporal(static_cast<string &&>(correo));
        correo = static_cast<string &&>(otro.correo);
        otro.correo = static_cast<string &&>(temporal);
    }
    {
        string temporal(static_cast<string &&>(ciudad));
        ciudad = static_cast<string &&>(otro.ciudad);
        otro.ciudad = static_cast<string &&>(temporal);
    }
    {
        string temporal(static_cast<string &&>(pais));
        pais = static_cast<string &&>(otro.pais);
        otro.pais = static_cast<string &&>(temporal);
    }
    {
        string temporal(static_cast<string &&>(ruta_perfil));
        ruta_perfil = static_cast<string &&>(otro.ruta_perfil);
        otro.ruta_perfil = static_cast<string &&>(temporal);
    }
    {
        Fecha temporal = nacimiento;
        RegistroLocal memoria_42(static_cast<unsigned int>(sizeof(temporal)));
        nacimiento = otro.nacimiento;
        otro.nacimiento = temporal;
    }
    {
        Fecha temporal = inscripcion;
        RegistroLocal memoria_43(static_cast<unsigned int>(sizeof(temporal)));
        inscripcion = otro.inscripcion;
        otro.inscripcion = temporal;
    }
    {
        unsigned char *temporal = solicitudes_recibidas;
        RegistroLocal memoria_44(static_cast<unsigned int>(sizeof(temporal)));
        solicitudes_recibidas = otro.solicitudes_recibidas;
        otro.solicitudes_recibidas = temporal;
    }
    {
        unsigned char temporal = bytes_indice;
        RegistroLocal memoria_45(static_cast<unsigned int>(sizeof(temporal)));
        bytes_indice = otro.bytes_indice;
        otro.bytes_indice = temporal;
    }
    {
        unsigned int temporal = cantidad_solicitudes;
        RegistroLocal memoria_46(static_cast<unsigned int>(sizeof(temporal)));
        cantidad_solicitudes = otro.cantidad_solicitudes;
        otro.cantidad_solicitudes = temporal;
    }
    {
        unsigned int temporal = capacidad_solicitudes;
        RegistroLocal memoria_47(static_cast<unsigned int>(sizeof(temporal)));
        capacidad_solicitudes = otro.capacidad_solicitudes;
        otro.capacidad_solicitudes = temporal;
    }
    {
        unsigned int temporal = cantidad_albumes;
        RegistroLocal memoria_48(static_cast<unsigned int>(sizeof(temporal)));
        cantidad_albumes = otro.cantidad_albumes;
        otro.cantidad_albumes = temporal;
    }
    {
        unsigned int temporal = capacidad_albumes;
        RegistroLocal memoria_49(static_cast<unsigned int>(sizeof(temporal)));
        capacidad_albumes = otro.capacidad_albumes;
        otro.capacidad_albumes = temporal;
    }
    {
        Album *temporal = albumes;
        RegistroLocal memoria_50(static_cast<unsigned int>(sizeof(temporal)));
        albumes = otro.albumes;
        otro.albumes = temporal;
    }
    {
        RegistroLocal memoria_51(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < cantidad_albumes; i++)
        {
            contar_iteracion();
            albumes[i].set_propietario(this);
        }
    }
    {
        RegistroLocal memoria_52(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < otro.cantidad_albumes; i++)
        {
            contar_iteracion();
            otro.albumes[i].set_propietario(&otro);
        }
    }
}
