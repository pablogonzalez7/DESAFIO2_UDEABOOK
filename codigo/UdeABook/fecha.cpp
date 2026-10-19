#include "recursos.h"
#include "fecha.h"
#include "archivo.h"

Fecha::Fecha()
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(this)));

    this->dia = 1;
    this->mes = 1;
    this->anio = 2000;
}

Fecha::Fecha(int d, int m, int a)
{
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(d)) + static_cast<unsigned int>(sizeof(m)) + static_cast<unsigned int>(sizeof(a)) + static_cast<unsigned int>(sizeof(this)));

    this->dia = 1;
    this->mes = 1;
    this->anio = 2000;

    this->set_fecha(d, m, a);
}

bool Fecha::es_bisiesto(int anio) const
{
    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(anio)) + static_cast<unsigned int>(sizeof(this)));

    return (anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0;
}

bool Fecha::fecha_valida(int d, int m, int a) const
{
    RegistroLocal memoria_4(static_cast<unsigned int>(sizeof(d)) + static_cast<unsigned int>(sizeof(m)) + static_cast<unsigned int>(sizeof(a)) + static_cast<unsigned int>(sizeof(this)));

    if (a < 1 || a > 9999 || m < 1 || m > 12 || d < 1)
        return false;

    int dias_mes = 31;
    RegistroLocal memoria_5(static_cast<unsigned int>(sizeof(dias_mes)));

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
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(d)) + static_cast<unsigned int>(sizeof(m)) + static_cast<unsigned int>(sizeof(a)) + static_cast<unsigned int>(sizeof(this)));

    if (!this->fecha_valida(d, m, a))
        return false;

    this->dia = static_cast<unsigned char>(d);
    this->mes = static_cast<unsigned char>(m);
    this->anio = static_cast<unsigned short>(a);

    return true;
}

int Fecha::get_dia() const
{
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(this)));

    return this->dia;
}

int Fecha::get_mes() const
{
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(this)));

    return this->mes;
}

int Fecha::get_anio() const
{
    RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(this)));

    return this->anio;
}

string Fecha::get_fecha() const
{
    RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(this)));

    string texto_dia = texto_numero(this->dia);
    RegistroLocal memoria_11(static_cast<unsigned int>(sizeof(texto_dia)));

    string texto_mes = texto_numero(this->mes);
    RegistroLocal memoria_12(static_cast<unsigned int>(sizeof(texto_mes)));

    string texto_anio = texto_numero(this->anio);
    RegistroLocal memoria_13(static_cast<unsigned int>(sizeof(texto_anio)));

    if (this->dia < 10)
        texto_dia = "0" + texto_dia;

    if (this->mes < 10)
        texto_mes = "0" + texto_mes;

    return texto_dia + "/" + texto_mes + "/" + texto_anio;
}

bool Fecha::es_mas_reciente(const Fecha &otra) const
{
    RegistroLocal memoria_14(static_cast<unsigned int>(sizeof(this)));

    if (this->anio != otra.anio)
        return this->anio > otra.anio;

    if (this->mes != otra.mes)
        return this->mes > otra.mes;

    return this->dia > otra.dia;
}
