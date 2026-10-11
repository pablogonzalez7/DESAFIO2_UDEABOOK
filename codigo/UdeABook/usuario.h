#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "fecha.h"

using namespace std;

class Usuario
{
private:
    string nombre, contra, correo, ciudad, pais;
    Fecha nacimiento, inscripcion;

    Usuario** solicitudes_recibidas;
    int cantidad_solicitudes;
    int capacidad_solicitudes;

public:
    Usuario();

    Usuario(string nombre,
            string correo,
            string contra,
            string ciudad,
            string pais,
            Fecha nacimiento,
            Fecha inscripcion);

    Usuario(const Usuario& otro);

    ~Usuario();

    const Usuario& operator=(const Usuario& otro);
    bool operator==(const Usuario& otro) const;

    void set_datos(string nombre,
                   string correo,
                   string contra,
                   string ciudad,
                   string pais,
                   Fecha nacimiento,
                   Fecha inscripcion);

    string get_nombre() const;
    string get_correo() const;
    string get_ciudad() const;
    string get_pais() const;

    Fecha get_nacimiento() const;
    Fecha get_inscripcion() const;

    bool verificar_contra(const string& intento) const;
};

#endif
