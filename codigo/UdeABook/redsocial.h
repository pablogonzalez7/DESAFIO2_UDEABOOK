#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include "usuario.h"
#include "mensaje.h"
#include "sugerencia.h"

using namespace std;

class RedSocial
{
private:
    Usuario* usuarios;
    int cantidad_usuarios;

    unsigned char* relaciones;
    int bytes_relaciones;
    int sobrantes;

    Mensaje* mensajes;
    int cantidad_mensajes;
    int capacidad_mensajes;
    bool mensajes_cargados;

    bool leer_bit(int posicion) const;
    int posicion_relacion(int pos1, int pos2) const;

    int cant_amigos_comunes(
        int candidato,
        const int* indices_amigos,
        int cantidad_amigos
        ) const;

    bool convertir_entero(const string& texto, int& numero) const;
    bool convertir_fecha(const string& texto, Fecha& fecha) const;
    bool pedir_fecha(const string& texto, Fecha& fecha) const;
    bool fecha_anterior(const Fecha& primera, const Fecha& segunda) const;

    bool mensaje_va_antes(const Mensaje& primero,
                          const Mensaje& segundo) const;

    void merge_sort_mensajes(Mensaje* auxiliar, int inicio, int fin);
    void mezclar_mensajes(Mensaje* auxiliar, int inicio,
                          int mitad, int fin);

    void ampliar_mensajes();
    void limites_mensajes(int pos_usuario, int& inicio, int& fin) const;
    void mostrar_mensaje(int posicion) const;

public:
    RedSocial();
    RedSocial(int cantidad_usuarios);
    ~RedSocial();

    RedSocial(const RedSocial& otra) = delete;
    RedSocial& operator=(const RedSocial& otra) = delete;

    int get_cantidad_usuarios() const;
    int get_cantidad_mensajes() const;

    Usuario* get_usuario(int posicion);
    int buscar_usuario(const string& nombre) const;

    bool relacion(int pos1, int pos2) const;
    bool agregar_amistad(int pos1, int pos2);

    Sugerencia* hallar_sugerencias(
        int pos_usuario,
        int k,
        int& cantidad
        );

    bool cargar_mensajes(const string& ruta);
    void ordenar_mensajes();

    bool enviar_mensaje(
        int pos_emisor,
        int pos_receptor,
        const string& contenido,
        const Fecha& fecha
        );

    void menu_enviar_mensaje(int pos_emisor);
    void leer_mensajes_nuevos(int pos_usuario);

    void buscar_mensajes_por_fecha(
        int pos_usuario,
        const Fecha& fecha_inicial,
        const Fecha& fecha_final
        ) const;

    void menu_buscar_mensajes_por_fecha(int pos_usuario) const;
};

#endif
