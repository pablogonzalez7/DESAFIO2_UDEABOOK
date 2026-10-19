#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "fecha.h"
#include "album.h"

using namespace std;

class Usuario
{
private:
    string nombre, contra, correo, ciudad, pais, ruta_perfil;
    Fecha nacimiento, inscripcion;

    unsigned char *solicitudes_recibidas;
    unsigned char bytes_indice;
    unsigned int cantidad_solicitudes;
    unsigned int capacidad_solicitudes;

    Album *albumes;
    unsigned int cantidad_albumes;
    unsigned int capacidad_albumes;

public:
    Usuario();

    Usuario(const string &nombre, const string &correo, const string &contra, const string &ciudad,
            const string &pais, Fecha nacimiento, Fecha inscripcion);

    Usuario(const Usuario &otro);

    ~Usuario();

    const Usuario &operator=(const Usuario &otro);
    bool operator==(const Usuario &otro) const;

    void set_datos(const string &nombre, const string &correo, const string &contra, const string &ciudad,
                   const string &pais, Fecha nacimiento, Fecha inscripcion);

    const string &get_nombre() const;
    const string &get_correo() const;
    const string &get_ciudad() const;
    const string &get_pais() const;

    Fecha get_nacimiento() const;
    Fecha get_inscripcion() const;

    bool verificar_contra(const string &intento) const;

    unsigned int get_cantidad_albumes() const;

    void set_ruta_perfil(const string &ruta);
    const string &get_ruta_perfil() const;
    const string &get_contra() const;
    bool reservar_solicitudes(unsigned int capacidad, unsigned int total_usuarios);
    unsigned int get_cantidad_solicitudes() const;
    unsigned int get_solicitud(unsigned int posicion) const;
    bool tiene_solicitud(unsigned int usuario) const;
    bool agregar_solicitud(unsigned int usuario);
    bool retirar_solicitud(unsigned int posicion);
    bool reservar_albumes(unsigned int cantidad);
    bool agregar_album(const Album &album);

    void intercambiar(Usuario &otro) noexcept;
    Album *get_album(unsigned int posicion);
    const Album *get_album(unsigned int posicion) const;
};

#endif
