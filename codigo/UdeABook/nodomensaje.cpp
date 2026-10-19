#include "recursos.h"
#include "nodomensaje.h"

NodoMensaje::NodoMensaje(const string &contenido, Usuario *emisor,
                         Usuario *receptor, const Fecha &fecha)
    : mensaje(contenido, emisor, receptor, fecha), siguiente(nullptr)
{
    RegistroLocal memoria_1(
        static_cast<unsigned int>(sizeof(emisor)) +
        static_cast<unsigned int>(sizeof(receptor)) +
        static_cast<unsigned int>(sizeof(this))
        );
}
