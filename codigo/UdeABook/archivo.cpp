#include "recursos.h"
#include "archivo.h"
#include <iostream>
unsigned int longitud_texto(const string &texto)
{
    unsigned int n = 0;
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(n)));

    while (texto[n] != '\0')
    {
        if (n == static_cast<unsigned int>(-1)) throw "Texto demasiado largo";
        contar_iteracion();
        n++;
    }
    return n;
}
string parte_texto(const string &texto, unsigned int inicio, unsigned int cantidad)
{
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(inicio)) + static_cast<unsigned int>(sizeof(cantidad)));

    string resultado;
    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(resultado)));

    unsigned int n = longitud_texto(texto);
    RegistroLocal memoria_4(static_cast<unsigned int>(sizeof(n)));

    if (inicio > n)
        return resultado;
    {
        RegistroLocal memoria_5(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = inicio; i < n && cantidad > 0; i++, cantidad--)
        {
            contar_iteracion();
            resultado += texto[i];
        }
    }
    return resultado;
}
bool leer_linea(ifstream &archivo, string &linea)
{
    linea = "";
    char c;
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(c)));

    bool leido = false;
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(leido)));

    while ((contar_llamada(1), archivo.get(c)))
    {
        contar_iteracion();
        leido = true;
        if (c == '\n')
            return true;
        if (c == '\0')
            throw "Caracter nulo en archivo";
        if (c != '\r')
            linea += c;
    }
    return leido;
}
bool tomar_campo(const string &linea, unsigned int &posicion, string &campo, bool ultimo)
{
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(ultimo)));

    campo = "";
    unsigned int n = longitud_texto(linea);
    RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(n)));

    if (posicion > n)
        return false;
    while (posicion < n && linea[posicion] != ',')
    {
        contar_iteracion();
        campo += linea[posicion++];
    }
    if (ultimo)
        return posicion == n;
    if (posicion == n)
        return false;
    posicion++;
    return true;
}
bool numero_natural(const string &texto, unsigned int &numero)
{
    numero = 0;
    if (texto == "")
        return false;
    unsigned int maximo = static_cast<unsigned int>(-1);
    RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(maximo)));

    {
        RegistroLocal memoria_11(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; texto[i] != '\0'; i++)
        {
            contar_iteracion();
            if (texto[i] < '0' || texto[i] > '9')
                return false;
            unsigned int digito = texto[i] - '0';
            RegistroLocal memoria_12(static_cast<unsigned int>(sizeof(digito)));

            if (numero > (maximo - digito) / 10)
                return false;
            numero = numero * 10 + digito;
        }
    }
    return true;
}
string texto_numero(unsigned int numero)
{
    RegistroLocal memoria_13(static_cast<unsigned int>(sizeof(numero)));

    char cifras[static_cast<unsigned int>(sizeof(unsigned int)) * 3 + 1];
    RegistroLocal memoria_14(static_cast<unsigned int>(sizeof(cifras)));

    unsigned int cantidad = 0;
    RegistroLocal memoria_15(static_cast<unsigned int>(sizeof(cantidad)));

    do
    {
        contar_iteracion();
        cifras[cantidad++] = char('0' + numero % 10);
        numero /= 10;
    } while (numero != 0);
    string texto;
    RegistroLocal memoria_16(static_cast<unsigned int>(sizeof(texto)));

    while (cantidad > 0)
    {
        contar_iteracion();
        texto += cifras[--cantidad];
    }
    return texto;
}
bool campo_simple(const string &texto)
{
    if (texto == "")
        return false;
    {
        RegistroLocal memoria_17(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; texto[i] != '\0'; i++)
        {
            contar_iteracion();
            if (texto[i] == ',' || texto[i] == '\n' || texto[i] == '\r')
                return false;
        }
    }
    return true;
}
Archivo::Archivo()
{
    RegistroLocal memoria_18(static_cast<unsigned int>(sizeof(this)));

    this->lineas = nullptr;
    this->cantidad_lineas = 0;
    this->capacidad_lineas = 0;
}
Archivo::Archivo(const string &ruta) : Archivo()
{
    RegistroLocal memoria_19(static_cast<unsigned int>(sizeof(this)));
    this->ruta = ruta;
}
Archivo::Archivo(const Archivo &otro)
{
    this->lineas = nullptr;
    this->cantidad_lineas = 0;
    this->capacidad_lineas = 0;

    RegistroLocal memoria_20(static_cast<unsigned int>(sizeof(this)));

    try
    {
        this->ruta = otro.ruta;
        if (otro.capacidad_lineas != 0)
            this->lineas =
                registrar_reserva(new string[otro.capacidad_lineas],
                                  (this->capacidad_lineas = otro.capacidad_lineas) * static_cast<unsigned int>(sizeof(string)));
        this->capacidad_lineas = otro.capacidad_lineas;
        {
            RegistroLocal memoria_21(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < otro.cantidad_lineas; i++)
            {
                contar_iteracion();
                this->lineas[i] = otro.lineas[i];
            }
        }
        this->cantidad_lineas = otro.cantidad_lineas;
    }
    catch (...)
    {
        (registrar_liberacion(this->lineas != nullptr ? this->capacidad_lineas * static_cast<unsigned int>(sizeof(string)) : 0),
         delete[] this->lineas);
        this->lineas = nullptr;
        throw;
    }
}
Archivo::~Archivo()
{
    RegistroLocal memoria_22(static_cast<unsigned int>(sizeof(this)));
    (registrar_liberacion(this->lineas != nullptr ? this->capacidad_lineas * static_cast<unsigned int>(sizeof(string)) : 0),
     delete[] this->lineas);
}
const Archivo &Archivo::operator=(const Archivo &otro)
{
    RegistroLocal memoria_23(static_cast<unsigned int>(sizeof(this)));

    if (this == &otro)
        return *this;
    Archivo copia(otro);
    RegistroLocal memoria_24(static_cast<unsigned int>(sizeof(copia)));

    this->ruta = copia.ruta;
    (registrar_liberacion(this->lineas != nullptr ? this->capacidad_lineas * static_cast<unsigned int>(sizeof(string)) : 0),
     delete[] this->lineas);
    this->lineas = copia.lineas;
    copia.lineas = nullptr;
    this->cantidad_lineas = copia.cantidad_lineas;
    this->capacidad_lineas = copia.capacidad_lineas;
    return *this;
}
bool Archivo::operator==(const Archivo &otro) const
{
    RegistroLocal memoria_25(static_cast<unsigned int>(sizeof(this)));

    if (this->ruta != otro.ruta || this->cantidad_lineas != otro.cantidad_lineas)
        return false;
    {
        RegistroLocal memoria_26(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < this->cantidad_lineas; i++)
        {
            contar_iteracion();
            if (this->lineas[i] != otro.lineas[i])
                return false;
        }
    }
    return true;
}
void Archivo::set_ruta(const string &ruta)
{
    RegistroLocal memoria_27(static_cast<unsigned int>(sizeof(this)));
    this->ruta = ruta;
}
string Archivo::get_ruta() const
{
    RegistroLocal memoria_28(static_cast<unsigned int>(sizeof(this)));
    return this->ruta;
}
unsigned int Archivo::get_cantidad_lineas() const
{
    RegistroLocal memoria_29(static_cast<unsigned int>(sizeof(this)));
    return this->cantidad_lineas;
}
const string &Archivo::get_linea(unsigned int posicion) const
{
    RegistroLocal memoria_30(static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_lineas)
        throw "Indice de linea invalido";
    return this->lineas[posicion];
}
bool Archivo::reservar(unsigned int capacidad)
{
    RegistroLocal memoria_31(static_cast<unsigned int>(sizeof(capacidad)) + static_cast<unsigned int>(sizeof(this)));

    if (this->lineas != nullptr || this->cantidad_lineas != 0)
        return false;
    if (capacidad > static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(string)))
        return false;
    try
    {
        if (capacidad != 0)
            this->lineas = registrar_reserva(new string[capacidad],
                                             (this->capacidad_lineas = capacidad) * static_cast<unsigned int>(sizeof(string)));
        this->capacidad_lineas = capacidad;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo reservar el arreglo de lineas.\n";
        }
        return false;
    }
}
bool Archivo::cargar()
{
    RegistroLocal memoria_32(static_cast<unsigned int>(sizeof(this)));

    unsigned int bytes_nuevas = 0;
    RegistroLocal memoria_33(static_cast<unsigned int>(sizeof(bytes_nuevas)));

    if (this->lineas != nullptr)
        return false;
    string *nuevas = nullptr;
    RegistroLocal memoria_34(static_cast<unsigned int>(sizeof(nuevas)));

    try
    {
        ifstream archivo(this->ruta);
        RegistroLocal memoria_35(static_cast<unsigned int>(sizeof(archivo)));
        contar_llamada(2);
        if (!archivo)
            return false;
        unsigned int total = 0;
        RegistroLocal memoria_36(static_cast<unsigned int>(sizeof(total)));

        string texto;
        RegistroLocal memoria_37(static_cast<unsigned int>(sizeof(texto)));

        while (leer_linea(archivo, texto))
        {
            contar_iteracion();
            if (total == static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(string)))
                return false;
            total++;
        }
        if ((contar_llamada(10), archivo.bad()))
            return false;
        (contar_llamada(4), archivo.clear());
        (contar_llamada(5), archivo.seekg(0));
        if (!archivo)
            return false;
        if (total != 0)
            nuevas = registrar_reserva(new string[total], (bytes_nuevas = (total) * static_cast<unsigned int>(sizeof(string))));
        {
            RegistroLocal memoria_38(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < total; i++)
            {
                contar_iteracion();
                if (!leer_linea(archivo, nuevas[i]))
                    throw "Archivo incompleto";
            }
        }
        if (leer_linea(archivo, texto) || (contar_llamada(10), archivo.bad()))
            throw "Archivo cambiado";
        this->lineas = nuevas;
        this->cantidad_lineas = total;
        this->capacidad_lineas = total;
        return true;
    }
    catch (...)
    {
        (registrar_liberacion(nuevas != nullptr ? bytes_nuevas : 0), delete[] nuevas);
        {
            contar_llamadas(9, 1);
            cout << "No se pudo cargar el archivo de lineas.\n";
        }
        return false;
    }
}
bool Archivo::guardar() const
{
    RegistroLocal memoria_39(static_cast<unsigned int>(sizeof(this)));

    try
    {
        ofstream archivo(this->ruta);
        RegistroLocal memoria_40(static_cast<unsigned int>(sizeof(archivo)));
        contar_llamada(2);
        if (!archivo)
            return false;
        {
            RegistroLocal memoria_41(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_lineas; i++)
            {
                contar_iteracion();
                {
                    contar_llamadas(9, 2);
                    archivo << this->lineas[i] << '\n';
                }
            }
        }
        (contar_llamada(3), archivo.close());
        return !(contar_llamada(10), archivo.fail());
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo guardar el archivo.\n";
        }
        return false;
    }
}
bool Archivo::agregar_linea(const string &texto)
{
    RegistroLocal memoria_42(static_cast<unsigned int>(sizeof(this)));

    if (this->cantidad_lineas == this->capacidad_lineas)
        return false;
    try
    {
        this->lineas[this->cantidad_lineas] = texto;
        this->cantidad_lineas++;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo almacenar la linea.\n";
        }
        return false;
    }
}
bool Archivo::modificar_linea(unsigned int posicion, const string &texto)
{
    RegistroLocal memoria_43(static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_lineas)
        return false;
    try
    {
        this->lineas[posicion] = texto;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo modificar la linea.\n";
        }
        return false;
    }
}
bool Archivo::eliminar_linea(unsigned int posicion)
{
    RegistroLocal memoria_44(static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    if (posicion >= this->cantidad_lineas)
        return false;
    try
    {
        {
            RegistroLocal memoria_45(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = posicion; i + 1 < this->cantidad_lineas; i++)
            {
                contar_iteracion();
                this->lineas[i] = this->lineas[i + 1];
            }
        }
        this->lineas[--this->cantidad_lineas] = "";
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo eliminar la linea.\n";
        }
        return false;
    }
}
