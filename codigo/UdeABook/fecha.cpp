
#include "fecha.h"
#include <sstream>
#include <iomanip>

Fecha::Fecha()
    : dia(1), mes(1), anio(2000)
{
}

Fecha::Fecha(int d, int m, int a)
    : dia(1), mes(1), anio(2000)
{
    set_fecha(d, m, a);
}

bool Fecha::es_bisiesto(int a) const
{
    return (a % 400 == 0) || (a % 4 == 0 && a % 100 != 0);
}

bool Fecha::fecha_valida(int d, int m, int a) const
{
    if (a < 1 || m < 1 || m > 12 || d < 1)
        return false;

    int dias_por_mes[12] = {31, 28, 31, 30, 31, 30,
                            31, 31, 30, 31, 30, 31};

    if (m == 2 && es_bisiesto(a))
        return d <= 29;

    return d <= dias_por_mes[m - 1];
}

bool Fecha::set_fecha(int d, int m, int a)
{
    if (!fecha_valida(d, m, a))
        return false;

    dia = d;
    mes = m;
    anio = a;

    return true;
}

int Fecha::get_dia() const
{
    return dia;
}

int Fecha::get_mes() const
{
    return mes;
}

int Fecha::get_anio() const
{
    return anio;
}

std::string Fecha::get_fecha() const
{
    std::ostringstream salida;

    salida << std::setfill('0')
           << std::setw(2) << dia << "/"
           << std::setw(2) << mes << "/"
           << std::setw(4) << anio;

    return salida.str();
}
