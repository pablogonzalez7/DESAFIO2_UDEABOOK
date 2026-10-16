#include "redsocial.h"

RedSocial::RedSocial()
{
    this->usuarios = nullptr;
    this->cantidad_usuarios = 0;
    this->relaciones = nullptr;
    this->bytes_relaciones = 0;
    this->sobrantes = 0;
}

RedSocial::RedSocial(int cantidad_usuarios)
{
    this->usuarios = nullptr;
    this->cantidad_usuarios = 0;
    this->relaciones = nullptr;
    this->bytes_relaciones = 0;
    this->sobrantes = 0;

    if (cantidad_usuarios <= 0)
        return;

    this->cantidad_usuarios = cantidad_usuarios;

    long long bits =
        1LL * cantidad_usuarios * (cantidad_usuarios - 1) / 2;

    this->bytes_relaciones = (bits + 7) / 8;
    this->sobrantes = this->bytes_relaciones * 8LL - bits;

    this->usuarios = new Usuario[this->cantidad_usuarios];

    if (this->bytes_relaciones > 0)
    {
        this->relaciones =
            new unsigned char[this->bytes_relaciones]();
    }
}

RedSocial::~RedSocial()
{
    delete[] this->usuarios;
    delete[] this->relaciones;
}

int RedSocial::get_cantidad_usuarios() const
{
    return this->cantidad_usuarios;
}

Usuario* RedSocial::get_usuario(int posicion)
{
    if (posicion < 0 || posicion >= this->cantidad_usuarios)
        return nullptr;

    return &this->usuarios[posicion];
}

bool RedSocial::leer_bit(int posicion) const
{
    int posicion_real = posicion + this->sobrantes;
    int byte = posicion_real / 8;
    int bit = 7 - posicion_real % 8;

    return (this->relaciones[byte] >> bit) & 1;
}

int RedSocial::posicion_relacion(int pos1, int pos2) const
{
    long long n = this->cantidad_usuarios - 1;
    long long m;

    if (pos2 > pos1)
    {
        m = n - pos1;

        return n * (n + 1) / 2
               - m * (m + 1) / 2
               + (pos2 - pos1) - 1;
    }

    m = n - pos2;

    return n * (n + 1) / 2
           - m * (m + 1) / 2
           + (pos1 - pos2) - 1;
}

bool RedSocial::relacion(int pos1, int pos2) const
{
    if (pos1 < 0 || pos2 < 0 ||
        pos1 >= this->cantidad_usuarios ||
        pos2 >= this->cantidad_usuarios ||
        pos1 == pos2)
    {
        return false;
    }

    int posicion = this->posicion_relacion(pos1, pos2);

    return this->leer_bit(posicion);
}

bool RedSocial::agregar_amistad(int pos1, int pos2)
{
    if (pos1 < 0 || pos2 < 0 ||
        pos1 >= this->cantidad_usuarios ||
        pos2 >= this->cantidad_usuarios ||
        pos1 == pos2)
    {
        return false;
    }

    int posicion_real =
        this->posicion_relacion(pos1, pos2) + this->sobrantes;

    int byte = posicion_real / 8;
    int bit = 7 - posicion_real % 8;

    this->relaciones[byte] |= (1u << bit);

    return true;
}

int RedSocial::cant_amigos_comunes(
    int candidato,
    const int* indices_amigos,
    int cantidad_amigos
    ) const
{
    int comunes = 0;

    for (int i = 0; i < cantidad_amigos; i++)
    {
        if (this->relacion(candidato, indices_amigos[i]))
            comunes++;
    }

    return comunes;
}

Sugerencia* RedSocial::hallar_sugerencias(
    int pos_usuario,
    int k,
    int& cantidad
    )
{
    cantidad = 0;

    if (pos_usuario < 0 ||
        pos_usuario >= this->cantidad_usuarios ||
        k <= 0)
    {
        return nullptr;
    }

    int cantidad_amigos = 0;

    for (int i = 0; i < this->cantidad_usuarios; i++)
    {
        if (i != pos_usuario && this->relacion(pos_usuario, i))
            cantidad_amigos++;
    }

    int posibles =
        this->cantidad_usuarios - cantidad_amigos - 1;

    if (cantidad_amigos == 0 || posibles == 0)
        return nullptr;

    if (k > posibles)
        k = posibles;

    int* indices_amigos = new int[cantidad_amigos];
    int guardados = 0;

    for (int i = 0; i < this->cantidad_usuarios; i++)
    {
        if (i != pos_usuario && this->relacion(pos_usuario, i))
        {
            indices_amigos[guardados] = i;
            guardados++;
        }
    }

    Sugerencia* resultado = new Sugerencia[k];

    for (int candidato = 0;
         candidato < this->cantidad_usuarios;
         candidato++)
    {
        if (candidato == pos_usuario ||
            this->relacion(pos_usuario, candidato))
        {
            continue;
        }

        int comunes = this->cant_amigos_comunes(
            candidato,
            indices_amigos,
            cantidad_amigos
            );

        if (comunes == 0)
            continue;

        if (cantidad == k &&
            comunes <= resultado[cantidad - 1].get_amigos_comunes())
        {
            continue;
        }

        int posicion;

        if (cantidad < k)
        {
            posicion = cantidad;
            cantidad++;
        }
        else
        {
            posicion = k - 1;
        }

        while (posicion > 0 &&
               comunes > resultado[posicion - 1].get_amigos_comunes())
        {
            resultado[posicion] = resultado[posicion - 1];
            posicion--;
        }

        resultado[posicion].set_usuario(
            &this->usuarios[candidato]
            );

        resultado[posicion].set_amigos_comunes(comunes);
    }

    delete[] indices_amigos;

    if (cantidad == 0)
    {
        delete[] resultado;
        return nullptr;
    }

    return resultado;
}
