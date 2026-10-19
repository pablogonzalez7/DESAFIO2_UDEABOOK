#ifndef REDSOCIAL_H
#define REDSOCIAL_H
#include <string>
#include <random>
#include "usuario.h"
#include "mensaje.h"
#include "sugerencia.h"
#include "archivo.h"
#include "nodomensaje.h"
using namespace std;
class RedSocial
{
private:
    minstd_rand generador;
    bool generador_iniciado;
    Usuario *usuarios;
    unsigned int cantidad_usuarios;
    unsigned char *relaciones;
    unsigned int bytes_relaciones;
    unsigned char sobrantes;
    Mensaje *mensajes;
    NodoMensaje *nuevos_mensajes;
    unsigned int cantidad_nuevos;
    void ordenar_nuevos();
    unsigned int cantidad_mensajes;
    unsigned int capacidad_mensajes;
    bool mensajes_cargados;
    bool albumes_cargados;
    bool error_fatal;
    string ruta_usuarios;
    bool calcular_matriz(unsigned int cantidad, unsigned int &bytes, unsigned char &padding) const;
    bool leer_bit(unsigned int posicion) const;
    unsigned int posicion_relacion(unsigned int pos1, unsigned int pos2) const;
    unsigned int cant_amigos_comunes(unsigned int candidato, const unsigned int *amigos, unsigned int cantidad) const;
    unsigned int buscar_usuario_en(const Usuario *lista, unsigned int cantidad, const string &nombre) const;
    bool convertir_entero(const string &texto, unsigned int &numero) const;
    bool convertir_fecha(const string &texto, Fecha &fecha) const;
    bool pedir_fecha(const string &texto, Fecha &fecha) const;
    bool fecha_anterior(const Fecha &primera, const Fecha &segunda) const;
    bool mensaje_va_antes(const Mensaje &primero, const Mensaje &segundo) const;
    void bajar_monticulo(unsigned int raiz, unsigned int cantidad);
    void limites_mensajes(unsigned int pos_usuario, unsigned int &inicio, unsigned int &fin) const;
    void mostrar_mensaje(unsigned int posicion) const;

public:
    RedSocial();
    RedSocial(unsigned int cantidad_usuarios);
    ~RedSocial();
    RedSocial(const RedSocial &otra) = delete;
    RedSocial &operator=(const RedSocial &otra) = delete;
    unsigned int get_cantidad_usuarios() const;
    unsigned int get_cantidad_mensajes() const;
    bool get_error_fatal() const;
    Usuario *get_usuario(unsigned int posicion);
    unsigned int buscar_usuario(const string &nombre) const;
    bool relacion(unsigned int pos1, unsigned int pos2) const;
    bool agregar_amistad(unsigned int pos1, unsigned int pos2);
    Sugerencia *hallar_sugerencias(unsigned int pos_usuario, unsigned int k, unsigned int &cantidad);
    bool enviar_solicitud(unsigned int emisor, unsigned int receptor);
    bool aceptar_solicitud(unsigned int receptor, unsigned int posicion);
    void menu_solicitudes(unsigned int usuario);
    void menu_sugerencias(unsigned int usuario);
    void mostrar_perfiles(unsigned int usuario, unsigned int *anteriores, unsigned int &cantidad_anteriores, bool renovar);
    bool cargar_usuarios(const string &ruta);
    bool guardar_usuarios();
    bool cargar_albumes(const string &ruta);
    bool guardar_albumes(const string &ruta);
    bool cargar_mensajes(const string &ruta);
    bool guardar_mensajes(const string &ruta);
    bool ordenar_mensajes();
    bool enviar_mensaje(unsigned int emisor, unsigned int receptor, const string &contenido, const Fecha &fecha);
    void menu_enviar_mensaje(unsigned int emisor);
    void leer_mensajes_nuevos(unsigned int usuario);
    void buscar_mensajes_por_fecha(unsigned int usuario, const Fecha &inicio, const Fecha &fin) const;
    void menu_buscar_mensajes_por_fecha(unsigned int usuario) const;
    void menu_visitar_perfil_amigo(unsigned int usuario);
};
#endif
