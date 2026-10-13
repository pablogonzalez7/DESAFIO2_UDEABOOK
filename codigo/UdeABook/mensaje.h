#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
using namespace std;

class Usuario;

class Mensaje
{
private:
    string contenido;
    Usuario* emisor;
    Usuario* receptor;
    bool leido;

public:
    Mensaje();
    Mensaje(string contenido, Usuario* emisor, Usuario* receptor);

    string get_contenido() const;
    Usuario* get_emisor() const;
    Usuario* get_receptor() const;
    bool get_leido() const;

    void marcar_leido();
};

#endif
