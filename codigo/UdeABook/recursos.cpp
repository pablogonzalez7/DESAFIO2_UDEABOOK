
#include "recursos.h"
#include <fstream>

Recursos::Recursos()
    : ruta(""), lineas(nullptr),
    cantidad_lineas(0), capacidad_lineas(0)
{
}

Recursos::Recursos(string ruta)
    : ruta(ruta), lineas(nullptr),
    cantidad_lineas(0), capacidad_lineas(0)
{
}

Recursos::Recursos(const Recursos& otro)
    : ruta(otro.ruta), lineas(nullptr),
    cantidad_lineas(otro.cantidad_lineas),
    capacidad_lineas(otro.capacidad_lineas)
{
    if (this->capacidad_lineas > 0) {

        this->lineas = new string[this->capacidad_lineas];

        for (int i = 0; i < this->cantidad_lineas; i++) {
            this->lineas[i] = otro.lineas[i];
        }
    }
}

Recursos::~Recursos()
{
    delete[] this->lineas;
}

const Recursos& Recursos::operator=(const Recursos& otro)
{
    if (this != &otro) {

        string* nuevas_lineas = nullptr;

        if (otro.capacidad_lineas > 0) {

            nuevas_lineas = new string[otro.capacidad_lineas];

            for (int i = 0; i < otro.cantidad_lineas; i++) {
                nuevas_lineas[i] = otro.lineas[i];
            }
        }

        delete[] this->lineas;

        this->ruta = otro.ruta;
        this->lineas = nuevas_lineas;
        this->cantidad_lineas = otro.cantidad_lineas;
        this->capacidad_lineas = otro.capacidad_lineas;
    }

    return *this;
}

bool Recursos::operator==(const Recursos& otro) const
{
    if (this->ruta != otro.ruta ||
        this->cantidad_lineas != otro.cantidad_lineas)
        return false;

    for (int i = 0; i < this->cantidad_lineas; i++) {
        if (this->lineas[i] != otro.lineas[i])
            return false;
    }

    return true;
}

void Recursos::set_ruta(string ruta)
{
    this->ruta = ruta;
}

string Recursos::get_ruta() const
{
    return this->ruta;
}

int Recursos::get_cantidad_lineas() const
{
    return this->cantidad_lineas;
}

string Recursos::get_linea(int posicion) const
{
    if (posicion < 0 || posicion >= this->cantidad_lineas)
        return "";

    return this->lineas[posicion];
}

bool Recursos::cargar()
{
    if (this->ruta.empty())
        return false;

    ifstream archivo(this->ruta.c_str());

    if (!archivo.is_open())
        return false;

    int total = 0;
    string texto;

    while (getline(archivo, texto)) {
        total++;
    }

    if (archivo.bad())
        return false;

    archivo.clear();
    archivo.seekg(0);

    if (!archivo)
        return false;

    string* nuevas_lineas = nullptr;

    if (total > 0)
        nuevas_lineas = new string[total];

    for (int i = 0; i < total; i++) {

        if (!getline(archivo, texto)) {
            delete[] nuevas_lineas;
            return false;
        }

        if (!texto.empty() &&
            texto[texto.size() - 1] == '\r') {
            texto.erase(texto.size() - 1);
        }

        nuevas_lineas[i] = texto;
    }

    if (archivo.bad()) {
        delete[] nuevas_lineas;
        return false;
    }

    delete[] this->lineas;

    this->lineas = nuevas_lineas;
    this->cantidad_lineas = total;
    this->capacidad_lineas = total;

    return true;
}

bool Recursos::guardar() const
{
    if (this->ruta.empty())
        return false;

    ofstream archivo(this->ruta.c_str());

    if (!archivo.is_open())
        return false;

    for (int i = 0; i < this->cantidad_lineas; i++) {

        archivo << this->lineas[i];

        if (i + 1 < this->cantidad_lineas)
            archivo << '\n';
    }

    archivo.close();

    return !archivo.fail();
}

bool Recursos::agregar_linea(string texto)
{
    if (this->cantidad_lineas == this->capacidad_lineas) {

        int nueva_capacidad = (this->capacidad_lineas == 0)
        ? 2 : this->capacidad_lineas * 2;

        string* nuevas_lineas = new string[nueva_capacidad];

        for (int i = 0; i < this->cantidad_lineas; i++) {
            nuevas_lineas[i] = this->lineas[i];
        }

        delete[] this->lineas;

        this->lineas = nuevas_lineas;
        this->capacidad_lineas = nueva_capacidad;
    }

    this->lineas[this->cantidad_lineas] = texto;
    this->cantidad_lineas++;

    return true;
}

bool Recursos::modificar_linea(int posicion, string texto)
{
    if (posicion < 0 || posicion >= this->cantidad_lineas)
        return false;

    this->lineas[posicion] = texto;

    return true;
}

bool Recursos::eliminar_linea(int posicion)
{
    if (posicion < 0 || posicion >= this->cantidad_lineas)
        return false;

    for (int i = posicion; i < this->cantidad_lineas - 1; i++) {
        this->lineas[i] = this->lineas[i + 1];
    }

    this->cantidad_lineas--;
    this->lineas[this->cantidad_lineas] = "";

    return true;
}
