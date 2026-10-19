#include "recursos.h"
#include <iostream>
Recursos *recursos_activos = nullptr;
unsigned int memoria_dinamica = 0;
unsigned int memoria_local = 0;
bool memoria_fuera_de_rango = false;
unsigned int sumar_memoria(unsigned int primero, unsigned int segundo)
{
    if (segundo > static_cast<unsigned int>(-1) - primero)
    {
        memoria_fuera_de_rango = true;
        return static_cast<unsigned int>(-1);
    }
    return primero + segundo;
}
unsigned int memoria_total()
{
    unsigned int globals = static_cast<unsigned int>(sizeof(recursos_activos) + sizeof(memoria_local) + sizeof(memoria_dinamica) + sizeof(memoria_fuera_de_rango));
    return sumar_memoria(sumar_memoria(memoria_local, memoria_dinamica), globals);
}
Recursos::Recursos(const char *nombre)
{
    this->funcionalidad = nombre;
    this->iteraciones = 0;
    for (unsigned char i = 0; i < 12; i++)
        this->llamadas[i] = 0;
    this->anterior = recursos_activos;
    recursos_activos = this;
    memoria_local = sumar_memoria(memoria_local, static_cast<unsigned int>(sizeof(Recursos)));
    this->maximo_memoria = memoria_total();
}
Recursos::~Recursos()
{
    if (!memoria_fuera_de_rango) memoria_local -= static_cast<unsigned int>(sizeof(Recursos));
    recursos_activos = this->anterior;
}
void Recursos::iterar()
{
    this->iteraciones++;
}
void Recursos::invocar(unsigned char funcion, unsigned int cantidad)
{
    if (funcion < 12)
        this->llamadas[funcion] = cantidad > static_cast<unsigned int>(-1) - this->llamadas[funcion] ? static_cast<unsigned int>(-1) : this->llamadas[funcion] + cantidad;
}
void Recursos::actualizar_maximo()
{
    unsigned int actual = memoria_total();
    if (actual > this->maximo_memoria)
        this->maximo_memoria = actual;
}
void Recursos::mostrar() const
{
    if (memoria_fuera_de_rango) cout << "Memoria: el conteo excedio el rango de unsigned int; los valores de memoria estan saturados.\n";
    const char *nombres[12] = {"getline (string)",
                               "get (fstream)",
                               "apertura de archivo (fstream)",
                               "close (fstream)",
                               "clear (iostream/fstream)",
                               "seekg (fstream)",
                               "random_device (random)",
                               "uniform_int_distribution (random)",
                               "seed (random)",
                               "insercion << (iostream/fstream)",
                               "estado del flujo (iostream/fstream)",
                               "ignore (iostream)"};
    cout << "\nPara esta funcionalidad: " << this->funcionalidad
         << "\nCantidad de iteraciones ejecutadas: " << this->iteraciones
         << "\nMemoria contabilizada al cierre: "
         << memoria_total()
         << " bytes"
         << "\nMaximo de memoria contabilizada: " << this->maximo_memoria << " bytes\n";
    for (unsigned char i = 0; i < 12; i++)
        if (this->llamadas[i] != 0)
            cout << "+ " << this->llamadas[i] << " invocaciones: " << nombres[i] << '\n';
    cout << "Incluye sizeof de objetos/locales y reservas propias. Excluye memoria interna de librerias y "
            "del asignador.\n\n";
}
RegistroLocal::RegistroLocal(unsigned int cantidad)
{
    this->bytes = sumar_memoria(cantidad, static_cast<unsigned int>(sizeof(RegistroLocal)));
    memoria_local = sumar_memoria(memoria_local, this->bytes);
    if (recursos_activos != nullptr)
        recursos_activos->actualizar_maximo();
}
RegistroLocal::~RegistroLocal()
{
    if (!memoria_fuera_de_rango) memoria_local -= this->bytes;
}
void contar_iteracion()
{
    if (recursos_activos != nullptr)
        recursos_activos->iterar();
}
void contar_llamada(unsigned char funcion)
{
    if (recursos_activos != nullptr)
        recursos_activos->invocar(funcion);
}
void registrar_reserva(unsigned int bytes)
{
    memoria_dinamica = sumar_memoria(memoria_dinamica, bytes);
    if (recursos_activos != nullptr)
        recursos_activos->actualizar_maximo();
}
void registrar_liberacion(unsigned int bytes)
{
    if (!memoria_fuera_de_rango) memoria_dinamica -= bytes;
}

Usuario *registrar_reserva(Usuario *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

Album *registrar_reserva(Album *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

Foto *registrar_reserva(Foto *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

Mensaje *registrar_reserva(Mensaje *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

Sugerencia *registrar_reserva(Sugerencia *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

NodoMensaje *registrar_reserva(NodoMensaje *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

string *registrar_reserva(string *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

unsigned int *registrar_reserva(unsigned int *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

unsigned char *registrar_reserva(unsigned char *puntero, unsigned int bytes)
{
    registrar_reserva(bytes);
    return puntero;
}

void contar_llamadas(unsigned char funcion, unsigned int cantidad)
{
    if (recursos_activos != nullptr)
        recursos_activos->invocar(funcion, cantidad);
}
