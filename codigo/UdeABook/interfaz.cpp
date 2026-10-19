#include "recursos.h"
#include "interfaz.h"
#include "archivo.h"
#include <iostream>
bool pedir_numero(const string &texto, unsigned int minimo, unsigned int maximo, unsigned int &numero)
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(minimo)) + static_cast<unsigned int>(sizeof(maximo)));

    string entrada;
    RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(entrada)));

    while (true)
    {
        contar_iteracion();
        {
            contar_llamadas(9, 1);
            cout << texto;
        }
        if (!(contar_llamada(0), getline(cin, entrada)))
            return false;
        if (numero_natural(entrada, numero) && numero >= minimo && numero <= maximo)
            return true;
        {
            contar_llamadas(9, 5);
            cout << "Valor invalido. Rango: " << minimo << " a " << maximo << '\n';
        }
    }
}
