#ifndef RECURSOS_H
#define RECURSOS_H
#include <string>
using namespace std;
class Recursos
{
private:
    const char *funcionalidad;
    unsigned long long iteraciones;
    unsigned int llamadas[12];
    unsigned int maximo_memoria;
    Recursos *anterior;

public:
    Recursos(const char *nombre);
    ~Recursos();
    Recursos(const Recursos &) = delete;
    Recursos &operator=(const Recursos &) = delete;
    void iterar();
    void invocar(unsigned char funcion, unsigned int cantidad = 1);
    void actualizar_maximo();
    void mostrar() const;
};
class RegistroLocal
{
private:
    unsigned int bytes;

public:
    RegistroLocal(unsigned int cantidad);
    ~RegistroLocal();
    RegistroLocal(const RegistroLocal &) = delete;
    RegistroLocal &operator=(const RegistroLocal &) = delete;
};
extern Recursos *recursos_activos;
extern unsigned int memoria_dinamica;
extern unsigned int memoria_local;
void contar_iteracion();
void contar_llamada(unsigned char funcion);
void contar_llamadas(unsigned char funcion, unsigned int cantidad);
void registrar_reserva(unsigned int bytes);
void registrar_liberacion(unsigned int bytes);
class Usuario;
class Album;
class Foto;
class Mensaje;
class Sugerencia;
class NodoMensaje;
Usuario *registrar_reserva(Usuario *puntero, unsigned int bytes);
Album *registrar_reserva(Album *puntero, unsigned int bytes);
Foto *registrar_reserva(Foto *puntero, unsigned int bytes);
Mensaje *registrar_reserva(Mensaje *puntero, unsigned int bytes);
Sugerencia *registrar_reserva(Sugerencia *puntero, unsigned int bytes);
NodoMensaje *registrar_reserva(NodoMensaje *puntero, unsigned int bytes);
string *registrar_reserva(string *puntero, unsigned int bytes);
unsigned int *registrar_reserva(unsigned int *puntero, unsigned int bytes);
unsigned char *registrar_reserva(unsigned char *puntero, unsigned int bytes);
#endif
