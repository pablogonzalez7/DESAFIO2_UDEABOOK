#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include "fecha.h"

using namespace std;

class Usuario;

class Mensaje
{
private:
    string contenido;
    Usuario* emisor;
    Usuario* receptor;
    Fecha fecha;
    bool leido;

public:
    Mensaje();

    Mensaje(string contenido, Usuario* emisor,
            Usuario* receptor, const Fecha& fecha,
            bool leido = false);

    Mensaje(const Mensaje& otro);

    const Mensaje& operator=(const Mensaje& otro);
    bool operator==(const Mensaje& otro) const;

    const string& get_contenido() const;
    Usuario* get_emisor() const;
    Usuario* get_receptor() const;
    const Fecha& get_fecha() const;
    bool get_leido() const;

    void set_datos(string contenido, Usuario* emisor,
                   Usuario* receptor, const Fecha& fecha,
                   bool leido);

    void marcar_leido();
};

#endif
