#ifndef SUGERENCIA_H
#define SUGERENCIA_H
#include <string>
using namespace std;

class Usuario;

class Sugerencia
{
private:
    Usuario *usuario;
    unsigned int amigos_comunes;

public:
    Sugerencia();
    Sugerencia(Usuario *usuario, unsigned int amigos_comunes);
    Sugerencia(const Sugerencia &otra);

    const Sugerencia &operator=(const Sugerencia &otra);
    bool operator==(const Sugerencia &otra) const;

    Usuario *get_usuario() const;
    unsigned int get_amigos_comunes() const;

    void set_usuario(Usuario *usuario);
    void set_amigos_comunes(unsigned int amigos_comunes);
};

#endif

