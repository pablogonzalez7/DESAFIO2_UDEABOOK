#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "foto.h"

using namespace std;

class Usuario;

class Album
{
private:
    string nombre, ruta;
    unsigned char privacidad;
    Usuario *propietario;

    Foto *fotos;
    unsigned int cantidad_fotos;
    unsigned int capacidad_fotos;

public:
    Album();

    Album(const string &nombre, Usuario *propietario, const string &ruta = "",
          const string &privacidad = "privado");

    Album(const Album &otro);

    ~Album();

    const Album &operator=(const Album &otro);
    bool operator==(const Album &otro) const;

    void set_nombre(const string &nombre);
    void set_ruta(const string &ruta);
    bool set_privacidad(const string &privacidad);
    void set_propietario(Usuario *propietario);

    const string &get_nombre() const;
    const string &get_ruta() const;
    string get_privacidad() const;
    Usuario *get_propietario() const;

    unsigned int get_cantidad_fotos() const;

    bool reservar_fotos(unsigned int cantidad);
    bool agregar_foto(const Foto &foto);
    bool eliminar_foto(unsigned int posicion);

    Foto *get_foto(unsigned int posicion);
    const Foto *get_foto(unsigned int posicion) const;

    bool puede_ver(Usuario *visitante, bool es_amigo) const;
    void ver_carrusel(Usuario *visitante, bool es_amigo);
};

#endif
