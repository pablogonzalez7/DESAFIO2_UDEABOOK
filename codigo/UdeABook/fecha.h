#ifndef FECHA_H
#define FECHA_H

#include <string>
using namespace std;

class Fecha
{
private:
    unsigned char dia;
    unsigned char mes;
    unsigned short anio;

    bool es_bisiesto(int anio) const;
    bool fecha_valida(int d, int m, int a) const;

public:
    Fecha();
    Fecha(int d, int m, int a);

    bool set_fecha(int d, int m, int a);

    int get_dia() const;
    int get_mes() const;
    int get_anio() const;

    string get_fecha() const;

    bool es_mas_reciente(const Fecha &otra) const;
};

#endif
