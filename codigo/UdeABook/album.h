
#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "foto.h"

using namespace std;

class Usuario;

class Album
{
private:
    string nombre;
    Usuario* propietario;

    Foto* fotos;
    int cantidad_fotos;
    int capacidad_fotos;

public:
    Album();

    Album(string nombre, Usuario* propietario);

    Album(const Album& otro);

    ~Album();

    const Album& operator=(const Album& otro);
    bool operator==(const Album& otro) const;

    void set_nombre(string nombre);
    string get_nombre() const;

    void set_propietario(Usuario* propietario);
    Usuario* get_propietario() const;

    int get_cantidad_fotos() const;

    bool agregar_foto(const Foto& foto);
    bool eliminar_foto(int posicion);

    const Foto* get_foto(int posicion) const;
};

#endif
