#include "fecha.h"

Fecha::Fecha()
{
    this->dia = 1;
    this->mes = 1;
    this->anio = 2000;
}

Fecha::Fecha(int d, int m, int a)
{
    this->dia = 1;
    this->mes = 1;
    this->anio = 2000;

    this->set_fecha(d, m, a);
}

bool Fecha::es_bisiesto(int anio) const
{
    return (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
}

bool Fecha::fecha_valida(int d, int m, int a) const
{
    if (a < 1 || m < 1 || m > 12 || d < 1)
        return false;

    int dias_mes = 31;

    if (m == 4 || m == 6 || m == 9 || m == 11)
        dias_mes = 30;
    else if (m == 2)
    {
        if (this->es_bisiesto(a))
            dias_mes = 29;
        else
            dias_mes = 28;
    }

    return d <= dias_mes;
}

bool Fecha::set_fecha(int d, int m, int a)
{
    if (!this->fecha_valida(d, m, a))
        return false;

    this->dia = d;
    this->mes = m;
    this->anio = a;

    return true;
}

int Fecha::get_dia() const
{
    return this->dia;
}

int Fecha::get_mes() const
{
    return this->mes;
}

int Fecha::get_anio() const
{
    return this->anio;
}

string Fecha::get_fecha() const
{
    string texto_dia = to_string(this->dia);
    string texto_mes = to_string(this->mes);
    string texto_anio = to_string(this->anio);

    if (this->dia < 10)
        texto_dia = "0" + texto_dia;

    if (this->mes < 10)
        texto_mes = "0" + texto_mes;

    return texto_dia + "/" + texto_mes + "/" + texto_anio;
}

bool Fecha::es_mas_reciente(const Fecha& otra) const
{
    if (this->anio != otra.anio)
        return this->anio > otra.anio;

    if (this->mes != otra.mes)
        return this->mes > otra.mes;

    return this->dia > otra.dia;
}
