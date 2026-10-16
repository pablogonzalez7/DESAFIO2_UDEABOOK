#ifndef SUGERENCIA_H
#define SUGERENCIA_H

class Usuario;

class Sugerencia
{
private:
    Usuario* usuario;
    int amigos_comunes;

public:
    Sugerencia();
    Sugerencia(Usuario* usuario, int amigos_comunes);
    Sugerencia(const Sugerencia& otra);

    const Sugerencia& operator=(const Sugerencia& otra);
    bool operator==(const Sugerencia& otra) const;

    Usuario* get_usuario() const;
    int get_amigos_comunes() const;

    void set_usuario(Usuario* usuario);
    void set_amigos_comunes(int amigos_comunes);
};

#endif
