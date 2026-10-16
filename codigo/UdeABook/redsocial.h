#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include "usuario.h"
#include "sugerencia.h"

class RedSocial
{
private:
    Usuario* usuarios;
    int cantidad_usuarios;

    unsigned char* relaciones;
    int bytes_relaciones;
    int sobrantes;

    bool leer_bit(int posicion) const;
    int posicion_relacion(int pos1, int pos2) const;

    int cant_amigos_comunes(
        int candidato,
        const int* indices_amigos,
        int cantidad_amigos
        ) const;

public:
    RedSocial();
    RedSocial(int cantidad_usuarios);
    ~RedSocial();

    RedSocial(const RedSocial& otra) = delete;
    RedSocial& operator=(const RedSocial& otra) = delete;

    int get_cantidad_usuarios() const;
    Usuario* get_usuario(int posicion);

    bool relacion(int pos1, int pos2) const;
    bool agregar_amistad(int pos1, int pos2);

    Sugerencia* hallar_sugerencias(
        int pos_usuario,
        int k,
        int& cantidad
        );
};

#endif
