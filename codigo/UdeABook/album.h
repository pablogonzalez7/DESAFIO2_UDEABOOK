
#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "foto.h"

using namespace std;

class Usuario;

class Album
{
private:
    string nombre, ruta, privacidad;
    Usuario* propietario;

    Foto* fotos;
    int cantidad_fotos;
    int capacidad_fotos;

public:
    Album();

    Album(string nombre, Usuario* propietario,
          string ruta = "", string privacidad = "privado");

    Album(const Album& otro);

    ~Album();

    const Album& operator=(const Album& otro);
    bool operator==(const Album& otro) const;

    void set_nombre(string nombre);
    void set_ruta(string ruta);
    bool set_privacidad(string privacidad);
    void set_propietario(Usuario* propietario);

    string get_nombre() const;
    string get_ruta() const;
    string get_privacidad() const;
    Usuario* get_propietario() const;

    int get_cantidad_fotos() const;

    bool agregar_foto(const Foto& foto);
    bool eliminar_foto(int posicion);

    Foto* get_foto(int posicion);
    const Foto* get_foto(int posicion) const;

    bool puede_ver(Usuario* visitante, bool es_amigo) const;
    void ver_carrusel(Usuario* visitante, bool es_amigo);
};

#endif
