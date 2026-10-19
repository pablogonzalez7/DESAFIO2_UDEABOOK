#ifndef NODOMENSAJE_H
#define NODOMENSAJE_H
#include "mensaje.h"
class NodoMensaje
{
public:
    Mensaje mensaje;
    NodoMensaje *siguiente;
    NodoMensaje(const string &contenido, Usuario *emisor, Usuario *receptor, const Fecha &fecha);
    NodoMensaje(const NodoMensaje &) = delete;
    NodoMensaje &operator=(const NodoMensaje &) = delete;
};
#endif
