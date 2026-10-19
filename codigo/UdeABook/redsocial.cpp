#include "recursos.h"
#include "redsocial.h"
#include "interfaz.h"
#include <iostream>
#include <fstream>
using namespace std;
RedSocial::RedSocial()
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(this)));

    generador_iniciado = false;
    nuevos_mensajes = nullptr;
    cantidad_nuevos = 0;
    this->usuarios = nullptr;
    this->cantidad_usuarios = 0;
    this->relaciones = nullptr;
    this->bytes_relaciones = 0;
    this->sobrantes = 0;
    this->mensajes = nullptr;
    this->cantidad_mensajes = 0;
    this->capacidad_mensajes = 0;
    this->mensajes_cargados = false;
    this->albumes_cargados = false;
    this->error_fatal = false;
}
RedSocial::RedSocial(unsigned int cantidad) : RedSocial()
{
    RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(cantidad)) + static_cast<unsigned int>(sizeof(this)));

    try
    {
        if (cantidad > static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(Usuario)) ||
            !this->calcular_matriz(cantidad, this->bytes_relaciones, this->sobrantes))
            throw "Cantidad demasiado grande";
        if (cantidad != 0)
            this->usuarios = registrar_reserva(new Usuario[cantidad],
                                               (this->cantidad_usuarios = cantidad) * static_cast<unsigned int>(sizeof(Usuario)));
        if (this->bytes_relaciones != 0)
            this->relaciones = registrar_reserva(new unsigned char[this->bytes_relaciones](),
                                                 this->bytes_relaciones * static_cast<unsigned int>(sizeof(unsigned char)));
        this->cantidad_usuarios = cantidad;
    }
    catch (...)
    {
        (registrar_liberacion(this->usuarios != nullptr ? this->cantidad_usuarios * static_cast<unsigned int>(sizeof(Usuario)) : 0),
         delete[] this->usuarios);
        (registrar_liberacion(this->relaciones != nullptr ? this->bytes_relaciones : 0),
         delete[] this->relaciones);
        this->usuarios = nullptr;
        this->relaciones = nullptr;
        {
            contar_llamadas(9, 1);
            cout << "No se pudo crear la red social.\n";
        }
        throw;
    }
}
RedSocial::~RedSocial()
{
    RegistroLocal memoria_4(static_cast<unsigned int>(sizeof(this)));

    while (nuevos_mensajes != nullptr)
    {
        contar_iteracion();
        NodoMensaje *borrar = nuevos_mensajes;
        RegistroLocal memoria_5(static_cast<unsigned int>(sizeof(borrar)));

        nuevos_mensajes = borrar->siguiente;
        (registrar_liberacion(borrar != nullptr ? static_cast<unsigned int>(sizeof(NodoMensaje)) : 0), delete borrar);
    }
    (registrar_liberacion(this->mensajes != nullptr ? this->capacidad_mensajes * static_cast<unsigned int>(sizeof(Mensaje)) : 0),
     delete[] this->mensajes);
    (registrar_liberacion(this->relaciones != nullptr ? this->bytes_relaciones : 0),
     delete[] this->relaciones);
    (registrar_liberacion(this->usuarios != nullptr ? this->cantidad_usuarios * static_cast<unsigned int>(sizeof(Usuario)) : 0),
     delete[] this->usuarios);
}
unsigned int RedSocial::get_cantidad_usuarios() const
{
    RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(this)));
    return this->cantidad_usuarios;
}
unsigned int RedSocial::get_cantidad_mensajes() const
{
    RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(this)));
    return this->cantidad_mensajes + this->cantidad_nuevos;
}
bool RedSocial::get_error_fatal() const
{
    RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(this)));
    return this->error_fatal;
}
Usuario *RedSocial::get_usuario(unsigned int posicion)
{
    RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(this)));

    return posicion < this->cantidad_usuarios ? &this->usuarios[posicion] : nullptr;
}
unsigned int RedSocial::buscar_usuario_en(const Usuario *lista, unsigned int cantidad, const string &nombre) const
{
    RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(lista)) + static_cast<unsigned int>(sizeof(cantidad)) + static_cast<unsigned int>(sizeof(this)));

    unsigned int inicio = 0;
    RegistroLocal memoria_11(static_cast<unsigned int>(sizeof(inicio)));

    unsigned int fin = cantidad;
    RegistroLocal memoria_12(static_cast<unsigned int>(sizeof(fin)));

    while (inicio < fin)
    {
        contar_iteracion();
        unsigned int mitad = inicio + (fin - inicio) / 2;
        RegistroLocal memoria_13(static_cast<unsigned int>(sizeof(mitad)));

        if (lista[mitad].get_nombre() < nombre)
            inicio = mitad + 1;
        else
            fin = mitad;
    }
    if (inicio < cantidad && lista[inicio].get_nombre() == nombre)
        return inicio;
    return static_cast<unsigned int>(-1);
}
unsigned int RedSocial::buscar_usuario(const string &nombre) const
{
    RegistroLocal memoria_14(static_cast<unsigned int>(sizeof(this)));

    return this->buscar_usuario_en(this->usuarios, this->cantidad_usuarios, nombre);
}
bool RedSocial::calcular_matriz(unsigned int cantidad, unsigned int &bytes, unsigned char &padding) const
{
    RegistroLocal memoria_15(static_cast<unsigned int>(sizeof(cantidad)) + static_cast<unsigned int>(sizeof(this)));

    bytes = 0;
    padding = 0;
    if (cantidad < 2)
        return true;
    unsigned int a = cantidad;
    RegistroLocal memoria_16(static_cast<unsigned int>(sizeof(a)));

    unsigned int b = cantidad - 1;
    RegistroLocal memoria_17(static_cast<unsigned int>(sizeof(b)));

    if (a % 2 == 0)
        a /= 2;
    else
        b /= 2;
    unsigned int maximo = static_cast<unsigned int>(-1);
    RegistroLocal memoria_18(static_cast<unsigned int>(sizeof(maximo)));

    if (a > maximo / b)
        return false;
    unsigned int bits = a * b;
    RegistroLocal memoria_19(static_cast<unsigned int>(sizeof(bits)));

    padding = static_cast<unsigned char>((8 - bits % 8) % 8);
    if (bits > maximo - padding)
        return false;
    bytes = bits / 8 + (bits % 8 != 0);
    return true;
}
unsigned int RedSocial::posicion_relacion(unsigned int pos1, unsigned int pos2) const
{
    RegistroLocal memoria_20(static_cast<unsigned int>(sizeof(pos1)) + static_cast<unsigned int>(sizeof(pos2)) + static_cast<unsigned int>(sizeof(this)));

    if (pos1 > pos2)
    {
        unsigned int temporal = pos1;
        RegistroLocal memoria_21(static_cast<unsigned int>(sizeof(temporal)));
        pos1 = pos2;
        pos2 = temporal;
    }
    unsigned int a = pos1;
    RegistroLocal memoria_22(static_cast<unsigned int>(sizeof(a)));

    unsigned int b = (this->cantidad_usuarios - 1) + (this->cantidad_usuarios - pos1);
    RegistroLocal memoria_23(static_cast<unsigned int>(sizeof(b)));

    if (a % 2 == 0)
        a /= 2;
    else
        b /= 2;
    return a * b + pos2 - pos1 - 1;
}
bool RedSocial::leer_bit(unsigned int posicion) const
{
    RegistroLocal memoria_24(static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    unsigned int real = posicion + this->sobrantes;
    RegistroLocal memoria_25(static_cast<unsigned int>(sizeof(real)));

    unsigned char bit = static_cast<unsigned char>(7 - real % 8);
    RegistroLocal memoria_26(static_cast<unsigned int>(sizeof(bit)));

    return ((this->relaciones[real / 8] >> bit) & 1u) != 0;
}
bool RedSocial::relacion(unsigned int pos1, unsigned int pos2) const
{
    RegistroLocal memoria_27(static_cast<unsigned int>(sizeof(pos1)) + static_cast<unsigned int>(sizeof(pos2)) + static_cast<unsigned int>(sizeof(this)));

    if (pos1 >= this->cantidad_usuarios || pos2 >= this->cantidad_usuarios || pos1 == pos2)
        return false;
    return this->leer_bit(this->posicion_relacion(pos1, pos2));
}
bool RedSocial::agregar_amistad(unsigned int pos1, unsigned int pos2)
{
    RegistroLocal memoria_28(static_cast<unsigned int>(sizeof(pos1)) + static_cast<unsigned int>(sizeof(pos2)) + static_cast<unsigned int>(sizeof(this)));

    if (pos1 >= this->cantidad_usuarios || pos2 >= this->cantidad_usuarios || pos1 == pos2)
        return false;
    unsigned int real = this->posicion_relacion(pos1, pos2) + this->sobrantes;
    RegistroLocal memoria_29(static_cast<unsigned int>(sizeof(real)));

    this->relaciones[real / 8] |= (1u << (7 - real % 8));
    return true;
}
unsigned int RedSocial::cant_amigos_comunes(unsigned int candidato, const unsigned int *indices_amigos,
                                            unsigned int cantidad_amigos) const
{
    RegistroLocal memoria_30(static_cast<unsigned int>(sizeof(candidato)) + static_cast<unsigned int>(sizeof(indices_amigos)) + static_cast<unsigned int>(sizeof(cantidad_amigos)) +
                             static_cast<unsigned int>(sizeof(this)));

    unsigned int comunes = 0;
    RegistroLocal memoria_31(static_cast<unsigned int>(sizeof(comunes)));

    {
        RegistroLocal memoria_32(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < cantidad_amigos; i++)
        {
            contar_iteracion();
            if (this->relacion(candidato, indices_amigos[i]))
            {
                comunes++;
            }
        }
    }

    return comunes;
}
Sugerencia *RedSocial::hallar_sugerencias(unsigned int pos_usuario, unsigned int k, unsigned int &cantidad)
{
    RegistroLocal memoria_33(static_cast<unsigned int>(sizeof(this)));

    unsigned int bytes_indices_amigos = 0;
    RegistroLocal memoria_34(static_cast<unsigned int>(sizeof(bytes_indices_amigos)));

    unsigned int bytes_resultado = 0;
    RegistroLocal memoria_35(static_cast<unsigned int>(sizeof(bytes_resultado)));

    cantidad = 0;

    if (pos_usuario >= this->cantidad_usuarios || k < 1 || k > 20)
    {
        return nullptr;
    }

    unsigned int cantidad_amigos = 0;
    RegistroLocal memoria_36(static_cast<unsigned int>(sizeof(cantidad_amigos)));

    {
        RegistroLocal memoria_37(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
        {
            contar_iteracion();
            if (this->relacion(pos_usuario, i))
            {
                cantidad_amigos++;
            }
        }
    }

    unsigned int posibles = this->cantidad_usuarios - cantidad_amigos - 1;
    RegistroLocal memoria_38(static_cast<unsigned int>(sizeof(posibles)));

    if (cantidad_amigos == 0 || posibles == 0)
    {
        return nullptr;
    }

    unsigned int *indices_amigos = nullptr;
    RegistroLocal memoria_39(static_cast<unsigned int>(sizeof(indices_amigos)));

    Sugerencia *resultado = nullptr;
    RegistroLocal memoria_40(static_cast<unsigned int>(sizeof(resultado)));

    try
    {
        if (cantidad_amigos > static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(unsigned int)))
            throw "Cantidad demasiado grande";
        indices_amigos = registrar_reserva(new unsigned int[cantidad_amigos],
                                           (bytes_indices_amigos = (cantidad_amigos) * static_cast<unsigned int>(sizeof(unsigned int))));
        unsigned int guardados = 0;
        RegistroLocal memoria_41(static_cast<unsigned int>(sizeof(guardados)));

        {
            RegistroLocal memoria_42(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
            {
                contar_iteracion();
                if (this->relacion(pos_usuario, i))
                {
                    indices_amigos[guardados] = i;
                    guardados++;
                }
            }
        }

        resultado = registrar_reserva(new Sugerencia[k], (bytes_resultado = (k) * static_cast<unsigned int>(sizeof(Sugerencia))));

        {
            RegistroLocal memoria_43(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int candidato = 0; candidato < this->cantidad_usuarios; candidato++)
            {
                contar_iteracion();
                if (candidato == pos_usuario || this->relacion(pos_usuario, candidato))
                {
                    continue;
                }

                unsigned int comunes = this->cant_amigos_comunes(candidato, indices_amigos, cantidad_amigos);
                RegistroLocal memoria_44(static_cast<unsigned int>(sizeof(comunes)));

                if (comunes == 0)
                {
                    continue;
                }

                if (cantidad == k && comunes <= resultado[k - 1].get_amigos_comunes())
                {
                    continue;
                }

                unsigned int posicion;
                RegistroLocal memoria_45(static_cast<unsigned int>(sizeof(posicion)));

                if (cantidad < k)
                {
                    posicion = cantidad;
                    cantidad++;
                }
                else
                {
                    posicion = k - 1;
                }

                while (posicion > 0 && comunes > resultado[posicion - 1].get_amigos_comunes())
                {
                    contar_iteracion();
                    resultado[posicion] = resultado[posicion - 1];
                    posicion--;
                }

                resultado[posicion].set_usuario(&this->usuarios[candidato]);
                resultado[posicion].set_amigos_comunes(comunes);
            }
        }

        (registrar_liberacion(indices_amigos != nullptr ? bytes_indices_amigos : 0), delete[] indices_amigos);
        indices_amigos = nullptr;

        if (cantidad == 0)
        {
            (registrar_liberacion(resultado != nullptr ? bytes_resultado : 0), delete[] resultado);
            return nullptr;
        }

        return resultado;
    }
    catch (...)
    {
        (registrar_liberacion(indices_amigos != nullptr ? bytes_indices_amigos : 0), delete[] indices_amigos);
        (registrar_liberacion(resultado != nullptr ? bytes_resultado : 0), delete[] resultado);
        cantidad = 0;
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron calcular las sugerencias.\n";
        }
        return nullptr;
    }
}

bool RedSocial::convertir_entero(const string &texto, unsigned int &numero) const
{
    RegistroLocal memoria_46(static_cast<unsigned int>(sizeof(this)));

    return numero_natural(texto, numero);
}
bool RedSocial::convertir_fecha(const string &texto, Fecha &fecha) const
{
    RegistroLocal memoria_47(static_cast<unsigned int>(sizeof(this)));

    if (longitud_texto(texto) != 10 || texto[4] != '-' || texto[7] != '-')
        return false;
    unsigned int anio, mes, dia;
    RegistroLocal memoria_48(static_cast<unsigned int>(sizeof(anio)) + static_cast<unsigned int>(sizeof(mes)) + static_cast<unsigned int>(sizeof(dia)));

    if (!numero_natural(parte_texto(texto, 0, 4), anio) || !numero_natural(parte_texto(texto, 5, 2), mes) ||
        !numero_natural(parte_texto(texto, 8, 2), dia))
        return false;
    return fecha.set_fecha(static_cast<int>(dia), static_cast<int>(mes), static_cast<int>(anio));
}
bool RedSocial::pedir_fecha(const string &texto, Fecha &fecha) const
{
    RegistroLocal memoria_49(static_cast<unsigned int>(sizeof(this)));

    string entrada;
    RegistroLocal memoria_50(static_cast<unsigned int>(sizeof(entrada)));

    while (true)
    {
        contar_iteracion();
        {
            contar_llamadas(9, 2);
            cout << texto << " (AAAA-MM-DD): ";
        }

        if (!(contar_llamada(0), getline(cin, entrada)))
        {
            return false;
        }

        if (this->convertir_fecha(entrada, fecha))
        {
            return true;
        }

        {
            contar_llamadas(9, 1);
            cout << "Fecha invalida. Intenta nuevamente.\n";
        }
    }
}
bool RedSocial::fecha_anterior(const Fecha &primera, const Fecha &segunda) const
{
    RegistroLocal memoria_51(static_cast<unsigned int>(sizeof(this)));

    if (primera.get_anio() != segunda.get_anio())
    {
        return primera.get_anio() < segunda.get_anio();
    }

    if (primera.get_mes() != segunda.get_mes())
    {
        return primera.get_mes() < segunda.get_mes();
    }

    return primera.get_dia() < segunda.get_dia();
}
bool RedSocial::mensaje_va_antes(const Mensaje &primero, const Mensaje &segundo) const
{
    RegistroLocal memoria_52(static_cast<unsigned int>(sizeof(this)));

    string receptor1 = primero.get_receptor()->get_nombre();
    RegistroLocal memoria_53(static_cast<unsigned int>(sizeof(receptor1)));

    string receptor2 = segundo.get_receptor()->get_nombre();
    RegistroLocal memoria_54(static_cast<unsigned int>(sizeof(receptor2)));

    if (receptor1 != receptor2)
    {
        return receptor1 < receptor2;
    }

    return this->fecha_anterior(segundo.get_fecha(), primero.get_fecha());
}

void RedSocial::bajar_monticulo(unsigned int raiz, unsigned int cantidad)
{
    RegistroLocal memoria_55(static_cast<unsigned int>(sizeof(raiz)) + static_cast<unsigned int>(sizeof(cantidad)) + static_cast<unsigned int>(sizeof(this)));

    while (raiz < cantidad / 2)
    {
        contar_iteracion();
        unsigned int hijo = raiz * 2 + 1;
        RegistroLocal memoria_56(static_cast<unsigned int>(sizeof(hijo)));

        if (hijo + 1 < cantidad && this->mensaje_va_antes(this->mensajes[hijo], this->mensajes[hijo + 1]))
            hijo++;
        if (!this->mensaje_va_antes(this->mensajes[raiz], this->mensajes[hijo]))
            break;
        this->mensajes[raiz].intercambiar(this->mensajes[hijo]);
        raiz = hijo;
    }
}
bool RedSocial::ordenar_mensajes()
{
    RegistroLocal memoria_57(static_cast<unsigned int>(sizeof(this)));

    try
    {
        {
            RegistroLocal memoria_58(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = this->cantidad_mensajes / 2; i > 0; i--)
            {
                contar_iteracion();
                this->bajar_monticulo(i - 1, this->cantidad_mensajes);
            }
        }
        {
            RegistroLocal memoria_59(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int fin = this->cantidad_mensajes; fin > 1; fin--)
            {
                contar_iteracion();
                this->mensajes[0].intercambiar(this->mensajes[fin - 1]);
                this->bajar_monticulo(0, fin - 1);
            }
        }
        return true;
    }
    catch (...)
    {
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron ordenar los mensajes.\n";
        }
        return false;
    }
}
bool RedSocial::cargar_mensajes(const string &ruta)
{
    RegistroLocal memoria_60(static_cast<unsigned int>(sizeof(this)));

    unsigned int bytes_nuevos = 0;
    RegistroLocal memoria_61(static_cast<unsigned int>(sizeof(bytes_nuevos)));

    if (this->mensajes_cargados || this->cantidad_mensajes != 0)
        return false;
    Mensaje *nuevos = nullptr;
    RegistroLocal memoria_62(static_cast<unsigned int>(sizeof(nuevos)));

    try
    {
        ifstream archivo(ruta);
        RegistroLocal memoria_63(static_cast<unsigned int>(sizeof(archivo)));
        contar_llamada(2);
        if (!archivo)
        {
            {
                contar_llamadas(9, 1);
                cout << "No se pudo abrir mensajes.\n";
            }
            return false;
        }
        unsigned int total = 0;
        RegistroLocal memoria_64(static_cast<unsigned int>(sizeof(total)));

        string linea;
        RegistroLocal memoria_65(static_cast<unsigned int>(sizeof(linea)));

        unsigned int maximo = static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(Mensaje));
        RegistroLocal memoria_66(static_cast<unsigned int>(sizeof(maximo)));

        while (leer_linea(archivo, linea))
        {
            contar_iteracion();
            if (linea == "")
                continue;
            if (total == maximo)
                throw "Demasiados mensajes";
            total++;
        }
        if ((contar_llamada(10), archivo.bad()))
            throw "Tamano invalido";
        unsigned int capacidad = total;
        RegistroLocal memoria_67(static_cast<unsigned int>(sizeof(capacidad)));

        if (capacidad != 0)
            nuevos =
                registrar_reserva(new Mensaje[capacidad], (bytes_nuevos = (capacidad) * static_cast<unsigned int>(sizeof(Mensaje))));
        (contar_llamada(4), archivo.clear());
        (contar_llamada(5), archivo.seekg(0));
        if (!archivo)
            throw "No se pudo reiniciar lectura";
        unsigned int cantidad = 0;
        RegistroLocal memoria_68(static_cast<unsigned int>(sizeof(cantidad)));

        bool ordenados = true;
        RegistroLocal memoria_69(static_cast<unsigned int>(sizeof(ordenados)));

        while (leer_linea(archivo, linea))
        {
            contar_iteracion();
            if (linea == "")
                continue;
            if (cantidad == total)
                throw "Archivo cambiado";
            string campos[4];
            RegistroLocal memoria_70(static_cast<unsigned int>(sizeof(campos)));

            unsigned int pos = 0;
            RegistroLocal memoria_71(static_cast<unsigned int>(sizeof(pos)));

            {
                RegistroLocal memoria_72(1 * static_cast<unsigned int>(sizeof(unsigned char)));
                for (unsigned char j = 0; j < 4; j++)
                {
                    contar_iteracion();
                    if (!tomar_campo(linea, pos, campos[j], false))
                        throw "Mensaje incompleto";
                }
            }
            string contenido = parte_texto(linea, pos, longitud_texto(linea) - pos);
            RegistroLocal memoria_73(static_cast<unsigned int>(sizeof(contenido)));

            unsigned int receptor = this->buscar_usuario(campos[0]);
            RegistroLocal memoria_74(static_cast<unsigned int>(sizeof(receptor)));

            unsigned int emisor = this->buscar_usuario(campos[1]);
            RegistroLocal memoria_75(static_cast<unsigned int>(sizeof(emisor)));

            Fecha fecha;
            RegistroLocal memoria_76(static_cast<unsigned int>(sizeof(fecha)));

            if (receptor >= this->cantidad_usuarios || emisor >= this->cantidad_usuarios ||
                !this->convertir_fecha(campos[2], fecha) || (campos[3] != "0" && campos[3] != "1") ||
                contenido == "" || longitud_texto(contenido) > 512)
                throw "Mensaje invalido";
            nuevos[cantidad].set_datos(contenido, &this->usuarios[emisor], &this->usuarios[receptor], fecha,
                                       campos[3] == "1");
            if (cantidad > 0 && mensaje_va_antes(nuevos[cantidad], nuevos[cantidad - 1]))
                ordenados = false;
            cantidad++;
        }
        if ((contar_llamada(10), archivo.bad()) || cantidad != total)
            throw "Error de lectura";
        this->mensajes = nuevos;
        nuevos = nullptr;
        this->cantidad_mensajes = cantidad;
        this->capacidad_mensajes = capacidad;
        this->mensajes_cargados = true;
        return ordenados || this->ordenar_mensajes();
    }
    catch (...)
    {
        (registrar_liberacion(nuevos != nullptr ? bytes_nuevos : 0), delete[] nuevos);
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron cargar los mensajes: datos, lectura o memoria.\n";
        }
        return false;
    }
}
bool RedSocial::enviar_mensaje(unsigned int pos_emisor, unsigned int pos_receptor, const string &contenido,
                               const Fecha &fecha)
{
    RegistroLocal memoria_77(static_cast<unsigned int>(sizeof(pos_emisor)) + static_cast<unsigned int>(sizeof(pos_receptor)) + static_cast<unsigned int>(sizeof(this)));

    if (this->error_fatal || !this->relacion(pos_emisor, pos_receptor) || contenido == "" ||
        longitud_texto(contenido) > 512)
        return false;
    {
        RegistroLocal memoria_78(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; contenido[i] != '\0'; i++)
        {
            contar_iteracion();
            if (contenido[i] == '\n' || contenido[i] == '\r')
                return false;
        }
    }
    try
    {
        if (cantidad_nuevos == static_cast<unsigned int>(-1) - cantidad_mensajes)
            throw "Demasiados mensajes";
        NodoMensaje *nodo = registrar_reserva(
            new NodoMensaje(contenido, &usuarios[pos_emisor], &usuarios[pos_receptor], fecha),
            static_cast<unsigned int>(sizeof(NodoMensaje)));
        RegistroLocal memoria_79(static_cast<unsigned int>(sizeof(nodo)));

        nodo->siguiente = nuevos_mensajes;
        nuevos_mensajes = nodo;
        cantidad_nuevos++;
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo reservar memoria para el mensaje.\n";
        }
        return false;
    }
}

void RedSocial::menu_enviar_mensaje(unsigned int pos_emisor)
{
    RegistroLocal memoria_80(static_cast<unsigned int>(sizeof(pos_emisor)) + static_cast<unsigned int>(sizeof(this)));

    try
    {

        if (pos_emisor >= this->cantidad_usuarios)
        {
            {
                contar_llamadas(9, 1);
                cout << "Usuario invalido.\n";
            }
            return;
        }

        unsigned int cantidad_amigos = 0;
        RegistroLocal memoria_81(static_cast<unsigned int>(sizeof(cantidad_amigos)));

        {
            contar_llamadas(9, 1);
            cout << "A que amigo quieres enviar un mensaje?\n";
        }

        {
            RegistroLocal memoria_82(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
            {
                contar_iteracion();
                if (this->relacion(pos_emisor, i))
                {
                    cantidad_amigos++;

                    {
                        contar_llamadas(9, 4);
                        cout << cantidad_amigos << ". " << this->usuarios[i].get_nombre() << '\n';
                    }
                }
            }
        }

        if (cantidad_amigos == 0)
        {
            {
                contar_llamadas(9, 1);
                cout << "No tienes amigos para enviar mensajes.\n";
            }
            return;
        }

        string entrada;
        RegistroLocal memoria_83(static_cast<unsigned int>(sizeof(entrada)));

        unsigned int opcion = 0;
        RegistroLocal memoria_84(static_cast<unsigned int>(sizeof(opcion)));

        while (true)
        {
            contar_iteracion();
            {
                contar_llamadas(9, 3);
                cout << "Selecciona un numero entre 1 y " << cantidad_amigos << ": ";
            }

            if (!(contar_llamada(0), getline(cin, entrada)))
            {
                return;
            }

            if (this->convertir_entero(entrada, opcion) && opcion >= 1 && opcion <= cantidad_amigos)
            {
                break;
            }

            {
                contar_llamadas(9, 1);
                cout << "Opcion invalida. Intenta nuevamente.\n";
            }
        }

        unsigned int pos_receptor = -1;
        RegistroLocal memoria_85(static_cast<unsigned int>(sizeof(pos_receptor)));

        unsigned int contador = 0;
        RegistroLocal memoria_86(static_cast<unsigned int>(sizeof(contador)));

        {
            RegistroLocal memoria_87(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
            {
                contar_iteracion();
                if (this->relacion(pos_emisor, i))
                {
                    contador++;

                    if (contador == opcion)
                    {
                        pos_receptor = i;
                        break;
                    }
                }
            }
        }

        string contenido;
        RegistroLocal memoria_88(static_cast<unsigned int>(sizeof(contenido)));

        while (true)
        {
            contar_iteracion();
            {
                contar_llamadas(9, 1);
                cout << "Escribe el mensaje, maximo 512 caracteres:\n";
            }

            if (!(contar_llamada(0), getline(cin, contenido)))
            {
                return;
            }

            if (contenido != "" && longitud_texto(contenido) <= 512)
            {
                break;
            }

            {
                contar_llamadas(9, 1);
                cout << "El mensaje debe tener entre 1 y 512 caracteres.\n";
            }
        }

        Fecha fecha;
        RegistroLocal memoria_89(static_cast<unsigned int>(sizeof(fecha)));

        if (!this->pedir_fecha("Fecha de envio", fecha))
        {
            return;
        }

        if (this->enviar_mensaje(pos_emisor, pos_receptor, contenido, fecha))
        {
            {
                contar_llamadas(9, 1);
                cout << "Mensaje enviado.\n";
            }
        }
        else
        {
            {
                contar_llamadas(9, 1);
                cout << "No se pudo enviar el mensaje.\n";
            }
        }
    }
    catch (...)
    {
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudo completar la operacion.\n";
        }
    }
}
void RedSocial::limites_mensajes(unsigned int pos_usuario, unsigned int &inicio, unsigned int &fin) const
{
    RegistroLocal memoria_90(static_cast<unsigned int>(sizeof(pos_usuario)) + static_cast<unsigned int>(sizeof(this)));

    inicio = 0;
    fin = 0;

    if (pos_usuario >= this->cantidad_usuarios)
    {
        return;
    }

    string nombre = this->usuarios[pos_usuario].get_nombre();
    RegistroLocal memoria_91(static_cast<unsigned int>(sizeof(nombre)));

    unsigned int izquierda = 0;
    RegistroLocal memoria_92(static_cast<unsigned int>(sizeof(izquierda)));

    unsigned int derecha = this->cantidad_mensajes;
    RegistroLocal memoria_93(static_cast<unsigned int>(sizeof(derecha)));

    while (izquierda < derecha)
    {
        contar_iteracion();
        unsigned int mitad = izquierda + (derecha - izquierda) / 2;
        RegistroLocal memoria_94(static_cast<unsigned int>(sizeof(mitad)));

        string receptor = this->mensajes[mitad].get_receptor()->get_nombre();
        RegistroLocal memoria_95(static_cast<unsigned int>(sizeof(receptor)));

        if (receptor < nombre)
        {
            izquierda = mitad + 1;
        }
        else
        {
            derecha = mitad;
        }
    }

    inicio = izquierda;
    derecha = this->cantidad_mensajes;

    while (izquierda < derecha)
    {
        contar_iteracion();
        unsigned int mitad = izquierda + (derecha - izquierda) / 2;
        RegistroLocal memoria_96(static_cast<unsigned int>(sizeof(mitad)));

        string receptor = this->mensajes[mitad].get_receptor()->get_nombre();
        RegistroLocal memoria_97(static_cast<unsigned int>(sizeof(receptor)));

        if (receptor <= nombre)
        {
            izquierda = mitad + 1;
        }
        else
        {
            derecha = mitad;
        }
    }

    fin = izquierda;
}
void RedSocial::mostrar_mensaje(unsigned int posicion) const
{
    RegistroLocal memoria_98(static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    const Mensaje &mensaje = this->mensajes[posicion];

    {
        contar_llamadas(9, 3);
        cout << "\nEmisor: " << mensaje.get_emisor()->get_nombre() << '\n';
    }

    {
        contar_llamadas(9, 3);
        cout << "Fecha: " << mensaje.get_fecha().get_fecha() << '\n';
    }

    {
        contar_llamadas(9, 3);
        cout << "Estado: " << (mensaje.get_leido() ? "Leido" : "No leido") << '\n';
    }

    {
        contar_llamadas(9, 3);
        cout << "Contenido: " << mensaje.get_contenido() << '\n';
    }
}
void RedSocial::leer_mensajes_nuevos(unsigned int pos_usuario)
{
    RegistroLocal memoria_99(static_cast<unsigned int>(sizeof(pos_usuario)) + static_cast<unsigned int>(sizeof(this)));

    try
    {

        if (pos_usuario >= this->cantidad_usuarios)
        {
            {
                contar_llamadas(9, 1);
                cout << "Usuario invalido.\n";
            }
            return;
        }

        unsigned int inicio;
        RegistroLocal memoria_100(static_cast<unsigned int>(sizeof(inicio)));

        unsigned int fin;
        RegistroLocal memoria_101(static_cast<unsigned int>(sizeof(fin)));

        this->limites_mensajes(pos_usuario, inicio, fin);

        unsigned int mostrados = 0;
        RegistroLocal memoria_102(static_cast<unsigned int>(sizeof(mostrados)));

        {
            RegistroLocal memoria_103(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = inicio; i < fin; i++)
            {
                contar_iteracion();
                if (!this->mensajes[i].get_leido())
                {
                    this->mostrar_mensaje(i);
                    this->mensajes[i].marcar_leido();
                    mostrados++;
                }
            }
        }

        if (mostrados == 0)
        {
            {
                contar_llamadas(9, 1);
                cout << "No tienes mensajes nuevos.\n";
            }
        }
        else
        {
            {
                contar_llamadas(9, 1);
                cout << "\nLos mensajes mostrados quedaron marcados como leidos.\n";
            }
        }
    }
    catch (...)
    {
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudo completar la operacion.\n";
        }
    }
}
void RedSocial::buscar_mensajes_por_fecha(unsigned int pos_usuario, const Fecha &fecha_inicial,
                                          const Fecha &fecha_final) const
{
    RegistroLocal memoria_104(static_cast<unsigned int>(sizeof(pos_usuario)) + static_cast<unsigned int>(sizeof(this)));

    if (pos_usuario >= this->cantidad_usuarios)
    {
        {
            contar_llamadas(9, 1);
            cout << "Usuario invalido.\n";
        }
        return;
    }

    if (this->fecha_anterior(fecha_final, fecha_inicial))
    {
        {
            contar_llamadas(9, 1);
            cout << "La fecha inicial no puede ser posterior a la final.\n";
        }
        return;
    }

    unsigned int inicio;
    RegistroLocal memoria_105(static_cast<unsigned int>(sizeof(inicio)));

    unsigned int fin;
    RegistroLocal memoria_106(static_cast<unsigned int>(sizeof(fin)));

    this->limites_mensajes(pos_usuario, inicio, fin);

    unsigned int izquierda = inicio;
    RegistroLocal memoria_107(static_cast<unsigned int>(sizeof(izquierda)));

    unsigned int derecha = fin;
    RegistroLocal memoria_108(static_cast<unsigned int>(sizeof(derecha)));

    while (izquierda < derecha)
    {
        contar_iteracion();
        unsigned int mitad = izquierda + (derecha - izquierda) / 2;
        RegistroLocal memoria_109(static_cast<unsigned int>(sizeof(mitad)));

        if (this->fecha_anterior(fecha_final, this->mensajes[mitad].get_fecha()))
        {
            izquierda = mitad + 1;
        }
        else
        {
            derecha = mitad;
        }
    }

    unsigned int mostrados = 0;
    RegistroLocal memoria_110(static_cast<unsigned int>(sizeof(mostrados)));

    {
        RegistroLocal memoria_111(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = izquierda; i < fin; i++)
        {
            contar_iteracion();
            if (this->fecha_anterior(this->mensajes[i].get_fecha(), fecha_inicial))
            {
                break;
            }

            this->mostrar_mensaje(i);
            mostrados++;
        }
    }

    if (mostrados == 0)
    {
        {
            contar_llamadas(9, 1);
            cout << "No tienes mensajes en ese rango de fechas.\n";
        }
    }
}

void RedSocial::menu_buscar_mensajes_por_fecha(unsigned int pos_usuario) const
{
    RegistroLocal memoria_112(static_cast<unsigned int>(sizeof(pos_usuario)) + static_cast<unsigned int>(sizeof(this)));

    if (pos_usuario >= this->cantidad_usuarios)
        return;
    Fecha inicial, final;
    RegistroLocal memoria_113(static_cast<unsigned int>(sizeof(inicial)) + static_cast<unsigned int>(sizeof(final)));

    if (!this->pedir_fecha("Fecha inicial", inicial))
        return;
    do
    {
        contar_iteracion();
        if (!this->pedir_fecha("Fecha final", final))
            return;
        if (!this->fecha_anterior(final, inicial))
            break;
        {
            contar_llamadas(9, 1);
            cout << "La fecha final no puede ser anterior a la inicial.\n";
        }
    } while (true);
    this->buscar_mensajes_por_fecha(pos_usuario, inicial, final);
}

void bajar_usuarios(Usuario *lista, unsigned int raiz, unsigned int cantidad)
{
    RegistroLocal memoria_114(static_cast<unsigned int>(sizeof(lista)) + static_cast<unsigned int>(sizeof(raiz)) + static_cast<unsigned int>(sizeof(cantidad)));

    while (raiz < cantidad / 2)
    {
        contar_iteracion();
        unsigned int hijo = raiz * 2 + 1;
        RegistroLocal memoria_115(static_cast<unsigned int>(sizeof(hijo)));

        if (hijo + 1 < cantidad && lista[hijo].get_nombre() < lista[hijo + 1].get_nombre())
            hijo++;
        if (!(lista[raiz].get_nombre() < lista[hijo].get_nombre()))
            break;
        lista[raiz].intercambiar(lista[hijo]);
        raiz = hijo;
    }
}
bool RedSocial::cargar_usuarios(const string &ruta)
{
    RegistroLocal memoria_116(static_cast<unsigned int>(sizeof(this)));

    unsigned int bytes_nuevos = 0;
    RegistroLocal memoria_117(static_cast<unsigned int>(sizeof(bytes_nuevos)));

    unsigned int bytes_bits = 0;
    RegistroLocal memoria_118(static_cast<unsigned int>(sizeof(bytes_bits)));

    if (this->usuarios != nullptr || this->mensajes_cargados)
        return false;
    Usuario *nuevos = nullptr;
    RegistroLocal memoria_119(static_cast<unsigned int>(sizeof(nuevos)));

    unsigned char *bits = nullptr;
    RegistroLocal memoria_120(static_cast<unsigned int>(sizeof(bits)));

    try
    {
        Archivo archivo(ruta);
        RegistroLocal memoria_121(static_cast<unsigned int>(sizeof(archivo)));

        if (!archivo.cargar())
            throw "No se pudo leer usuarios";
        unsigned int lineas = archivo.get_cantidad_lineas();
        RegistroLocal memoria_122(static_cast<unsigned int>(sizeof(lineas)));

        if (lineas == 0 || lineas % 3 != 0)
            throw "Se requieren tres lineas por usuario";
        unsigned int total = lineas / 3;
        RegistroLocal memoria_123(static_cast<unsigned int>(sizeof(total)));

        if (total > static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(Usuario)))
            throw "Demasiados usuarios";
        unsigned int bytes;
        RegistroLocal memoria_124(static_cast<unsigned int>(sizeof(bytes)));

        unsigned char padding;
        RegistroLocal memoria_125(static_cast<unsigned int>(sizeof(padding)));

        if (!this->calcular_matriz(total, bytes, padding))
            throw "Matriz demasiado grande";
        nuevos = registrar_reserva(new Usuario[total], (bytes_nuevos = (total) * static_cast<unsigned int>(sizeof(Usuario))));
        {
            RegistroLocal memoria_126(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < total; i++)
            {
                contar_iteracion();
                string campos[8];
                RegistroLocal memoria_127(static_cast<unsigned int>(sizeof(campos)));

                unsigned int pos = 0;
                RegistroLocal memoria_128(static_cast<unsigned int>(sizeof(pos)));

                const string &linea = archivo.get_linea(i * 3);
                {
                    RegistroLocal memoria_129(1 * static_cast<unsigned int>(sizeof(unsigned char)));
                    for (unsigned char j = 0; j < 8; j++)
                    {
                        contar_iteracion();
                        if (!tomar_campo(linea, pos, campos[j], j == 7) || campos[j] == "")
                            throw "Datos personales invalidos";
                    }
                }
                Fecha nacimiento, inscripcion;
                RegistroLocal memoria_130(static_cast<unsigned int>(sizeof(nacimiento)) + static_cast<unsigned int>(sizeof(inscripcion)));

                if (!this->convertir_fecha(campos[5], nacimiento) ||
                    !this->convertir_fecha(campos[6], inscripcion))
                    throw "Fecha invalida";

                nuevos[i].set_datos(campos[0], campos[1], campos[2], campos[3], campos[4], nacimiento,
                                    inscripcion);
                nuevos[i].set_ruta_perfil(campos[7]);
            }
        }
        bool ordenados = true;
        RegistroLocal memoria_131(static_cast<unsigned int>(sizeof(ordenados)));

        {
            RegistroLocal memoria_132(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 1; i < total; i++)
            {
                contar_iteracion();
                if (nuevos[i].get_nombre() < nuevos[i - 1].get_nombre())
                    ordenados = false;
            }
        }
        if (!ordenados)
        {
            {
                RegistroLocal memoria_133(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                for (unsigned int paso = total / 2; paso > 0; paso--)
                {
                    contar_iteracion();
                    bajar_usuarios(nuevos, paso - 1, total);
                }
            }
            {
                RegistroLocal memoria_134(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                for (unsigned int fin = total; fin > 1; fin--)
                {
                    contar_iteracion();
                    nuevos[0].intercambiar(nuevos[fin - 1]);
                    bajar_usuarios(nuevos, 0, fin - 1);
                }
            }
        }
        {
            RegistroLocal memoria_135(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 1; i < total; i++)
            {
                contar_iteracion();
                if (nuevos[i].get_nombre() == nuevos[i - 1].get_nombre())
                    throw "Usuario repetido";
            }
        }
        if (bytes != 0)
            bits =
                registrar_reserva(new unsigned char[bytes](), (bytes_bits = (bytes) * static_cast<unsigned int>(sizeof(unsigned char))));
        {
            RegistroLocal memoria_136(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int registro = 0; registro < total; registro++)
            {
                contar_iteracion();
                unsigned int posicion_nombre = 0;
                RegistroLocal memoria_137(static_cast<unsigned int>(sizeof(posicion_nombre)));

                string nombre_registro;
                RegistroLocal memoria_138(static_cast<unsigned int>(sizeof(nombre_registro)));

                tomar_campo(archivo.get_linea(registro * 3), posicion_nombre, nombre_registro, false);
                unsigned int i = buscar_usuario_en(nuevos, total, nombre_registro);
                RegistroLocal memoria_139(static_cast<unsigned int>(sizeof(i)));

                const string &lista = archivo.get_linea(registro * 3 + 1);
                unsigned int pos = 0;
                RegistroLocal memoria_140(static_cast<unsigned int>(sizeof(pos)));

                unsigned int n = longitud_texto(lista);
                RegistroLocal memoria_141(static_cast<unsigned int>(sizeof(n)));

                while (pos < n)
                {
                    contar_iteracion();
                    string nombre;
                    RegistroLocal memoria_142(static_cast<unsigned int>(sizeof(nombre)));

                    while (pos < n && lista[pos] != ',')
                    {
                        contar_iteracion();
                        nombre += lista[pos++];
                    }
                    if (nombre == "")
                        throw "Amigo vacio";
                    if (pos < n && ++pos == n)
                        throw "Coma final en amigos";
                    unsigned int amigo = this->buscar_usuario_en(nuevos, total, nombre);
                    RegistroLocal memoria_143(static_cast<unsigned int>(sizeof(amigo)));

                    if (amigo >= total || amigo == i)
                        throw "Amigo invalido";
                    unsigned int menor = i < amigo ? i : amigo;
                    RegistroLocal memoria_144(static_cast<unsigned int>(sizeof(menor)));

                    unsigned int mayor = i < amigo ? amigo : i;
                    RegistroLocal memoria_145(static_cast<unsigned int>(sizeof(mayor)));

                    unsigned int a = menor;
                    RegistroLocal memoria_146(static_cast<unsigned int>(sizeof(a)));

                    unsigned int b = (total - 1) + (total - menor);
                    RegistroLocal memoria_147(static_cast<unsigned int>(sizeof(b)));

                    if (a % 2 == 0)
                        a /= 2;
                    else
                        b /= 2;
                    unsigned int real = a * b + mayor - menor - 1 + padding;
                    RegistroLocal memoria_148(static_cast<unsigned int>(sizeof(real)));

                    bits[real / 8] |= (1u << (7 - real % 8));
                }
            }
        }
        {
            RegistroLocal memoria_149(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int registro = 0; registro < total; registro++)
            {
                contar_iteracion();
                unsigned int posicion_nombre = 0;
                RegistroLocal memoria_150(static_cast<unsigned int>(sizeof(posicion_nombre)));

                string nombre_registro;
                RegistroLocal memoria_151(static_cast<unsigned int>(sizeof(nombre_registro)));

                tomar_campo(archivo.get_linea(registro * 3), posicion_nombre, nombre_registro, false);
                unsigned int i = buscar_usuario_en(nuevos, total, nombre_registro);
                RegistroLocal memoria_152(static_cast<unsigned int>(sizeof(i)));

                const string &lista = archivo.get_linea(registro * 3 + 2);
                unsigned int n = longitud_texto(lista);
                RegistroLocal memoria_153(static_cast<unsigned int>(sizeof(n)));

                unsigned int cantidad = n == 0 ? 0 : 1;
                RegistroLocal memoria_154(static_cast<unsigned int>(sizeof(cantidad)));

                {
                    RegistroLocal memoria_155(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                    for (unsigned int j = 0; j < n; j++)
                    {
                        contar_iteracion();
                        if (lista[j] == ',')
                            cantidad++;
                    }
                }
                if (cantidad > total - 1)
                    throw "Demasiadas solicitudes";
                unsigned int capacidad = cantidad < total - 1 ? cantidad + 1 : cantidad;
                RegistroLocal memoria_156(static_cast<unsigned int>(sizeof(capacidad)));

                if (!nuevos[i].reservar_solicitudes(capacidad, total))
                    throw "No se reservaron solicitudes";
                unsigned int pos = 0;
                RegistroLocal memoria_157(static_cast<unsigned int>(sizeof(pos)));

                while (pos < n)
                {
                    contar_iteracion();
                    string nombre;
                    RegistroLocal memoria_158(static_cast<unsigned int>(sizeof(nombre)));

                    while (pos < n && lista[pos] != ',')
                    {
                        contar_iteracion();
                        nombre += lista[pos++];
                    }
                    if (nombre == "")
                        throw "Solicitante vacio";
                    if (pos < n && ++pos == n)
                        throw "Coma final en solicitudes";
                    unsigned int solicitante = this->buscar_usuario_en(nuevos, total, nombre);
                    RegistroLocal memoria_159(static_cast<unsigned int>(sizeof(solicitante)));

                    if (solicitante >= total || solicitante == i)
                        throw "Solicitante invalido";
                    unsigned int menor = i < solicitante ? i : solicitante;
                    RegistroLocal memoria_160(static_cast<unsigned int>(sizeof(menor)));

                    unsigned int mayor = i < solicitante ? solicitante : i;
                    RegistroLocal memoria_161(static_cast<unsigned int>(sizeof(mayor)));

                    unsigned int a = menor, b = (total - 1) + (total - menor);
                    RegistroLocal memoria_162(static_cast<unsigned int>(sizeof(a)) + static_cast<unsigned int>(sizeof(b)));

                    if (a % 2 == 0)
                        a /= 2;
                    else
                        b /= 2;
                    unsigned int real = a * b + mayor - menor - 1 + padding;
                    RegistroLocal memoria_163(static_cast<unsigned int>(sizeof(real)));

                    if (((bits[real / 8] >> (7 - real % 8)) & 1u) != 0 ||
                        nuevos[solicitante].tiene_solicitud(i) || !nuevos[i].agregar_solicitud(solicitante))
                        throw "Solicitud duplicada, cruzada o entre amigos";
                }
            }
        }
        this->ruta_usuarios = ruta;
        this->usuarios = nuevos;
        this->cantidad_usuarios = total;
        this->relaciones = bits;
        this->bytes_relaciones = bytes;
        this->sobrantes = padding;
        return true;
    }
    catch (...)
    {
        (registrar_liberacion(nuevos != nullptr ? bytes_nuevos : 0), delete[] nuevos);
        (registrar_liberacion(bits != nullptr ? bytes_bits : 0), delete[] bits);
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron cargar usuarios: revise datos, archivo o memoria.\n";
        }
        return false;
    }
}
void escribir_fecha_archivo(ofstream &archivo, const Fecha &fecha)
{
    int anio = fecha.get_anio();
    RegistroLocal memoria_164(static_cast<unsigned int>(sizeof(anio)));

    if (anio < 1000)
    {
        contar_llamadas(9, 1);
        archivo << '0';
    }
    if (anio < 100)
    {
        contar_llamadas(9, 1);
        archivo << '0';
    }
    if (anio < 10)
    {
        contar_llamadas(9, 1);
        archivo << '0';
    }
    {
        contar_llamadas(9, 2);
        archivo << anio << '-';
    }
    if (fecha.get_mes() < 10)
    {
        contar_llamadas(9, 1);
        archivo << '0';
    }
    {
        contar_llamadas(9, 2);
        archivo << fecha.get_mes() << '-';
    }
    if (fecha.get_dia() < 10)
    {
        contar_llamadas(9, 1);
        archivo << '0';
    }
    {
        contar_llamadas(9, 1);
        archivo << fecha.get_dia();
    }
}
bool RedSocial::guardar_usuarios()
{
    RegistroLocal memoria_165(static_cast<unsigned int>(sizeof(this)));

    if (this->error_fatal || this->ruta_usuarios == "")
        return false;
    try
    {
        ofstream archivo(this->ruta_usuarios);
        RegistroLocal memoria_166(static_cast<unsigned int>(sizeof(archivo)));
        contar_llamada(2);
        if (!archivo)
            throw "No se pudo abrir usuarios";
        {
            RegistroLocal memoria_167(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
            {
                contar_iteracion();
                const Usuario &u = this->usuarios[i];
                {
                    contar_llamadas(9, 10);
                    archivo << u.get_nombre() << ',' << u.get_correo() << ',' << u.get_contra() << ','
                            << u.get_ciudad() << ',' << u.get_pais() << ',';
                }
                escribir_fecha_archivo(archivo, u.get_nacimiento());
                {
                    contar_llamadas(9, 1);
                    archivo << ',';
                }
                escribir_fecha_archivo(archivo, u.get_inscripcion());
                {
                    contar_llamadas(9, 3);
                    archivo << ',' << u.get_ruta_perfil() << '\n';
                }
                bool primero = true;
                RegistroLocal memoria_168(static_cast<unsigned int>(sizeof(primero)));

                {
                    RegistroLocal memoria_169(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                    for (unsigned int j = 0; j < this->cantidad_usuarios; j++)
                    {
                        contar_iteracion();
                        if (!this->relacion(i, j))
                            continue;
                        if (!primero)
                        {
                            contar_llamadas(9, 1);
                            archivo << ',';
                        }
                        {
                            contar_llamadas(9, 1);
                            archivo << this->usuarios[j].get_nombre();
                        }
                        primero = false;
                    }
                }
                {
                    contar_llamadas(9, 1);
                    archivo << '\n';
                }
                {
                    RegistroLocal memoria_170(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                    for (unsigned int j = 0; j < u.get_cantidad_solicitudes(); j++)
                    {
                        contar_iteracion();
                        if (j != 0)
                        {
                            contar_llamadas(9, 1);
                            archivo << ',';
                        }
                        {
                            contar_llamadas(9, 1);
                            archivo << this->usuarios[u.get_solicitud(j)].get_nombre();
                        }
                    }
                }
                {
                    contar_llamadas(9, 1);
                    archivo << '\n';
                }
            }
        }
        (contar_llamada(3), archivo.close());
        if ((contar_llamada(10), archivo.fail()))
            throw "Error al escribir usuarios";
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron guardar completamente los usuarios.\n";
        }
        return false;
    }
}
bool RedSocial::guardar_mensajes(const string &ruta)
{
    RegistroLocal memoria_171(static_cast<unsigned int>(sizeof(this)));

    if (this->error_fatal || !this->mensajes_cargados)
        return false;
    try
    {
        ofstream archivo(ruta);
        RegistroLocal memoria_172(static_cast<unsigned int>(sizeof(archivo)));
        contar_llamada(2);
        if (!archivo)
            throw "No se pudo abrir mensajes";
        ordenar_nuevos();
        unsigned int i = 0;
        RegistroLocal memoria_173(static_cast<unsigned int>(sizeof(i)));

        NodoMensaje *nodo = nuevos_mensajes;
        RegistroLocal memoria_174(static_cast<unsigned int>(sizeof(nodo)));

        while (i < cantidad_mensajes || nodo != nullptr)
        {
            contar_iteracion();
            const Mensaje *elegido;
            RegistroLocal memoria_175(static_cast<unsigned int>(sizeof(elegido)));

            if (nodo != nullptr && (i == cantidad_mensajes || mensaje_va_antes(nodo->mensaje, mensajes[i])))
            {
                elegido = &nodo->mensaje;
                nodo = nodo->siguiente;
            }
            else
                elegido = &mensajes[i++];
            const Mensaje &m = *elegido;
            {
                contar_llamadas(9, 4);
                archivo << m.get_receptor()->get_nombre() << ',' << m.get_emisor()->get_nombre() << ',';
            }
            escribir_fecha_archivo(archivo, m.get_fecha());
            {
                contar_llamadas(9, 5);
                archivo << ',' << (m.get_leido() ? 1 : 0) << ',' << m.get_contenido() << '\n';
            }
        }
        (contar_llamada(3), archivo.close());
        if ((contar_llamada(10), archivo.fail()))
            throw "Error al escribir mensajes";
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron guardar completamente los mensajes.\n";
        }
        return false;
    }
}
bool RedSocial::cargar_albumes(const string &ruta)
{
    RegistroLocal memoria_176(static_cast<unsigned int>(sizeof(this)));

    unsigned int bytes_cantidades = 0;
    RegistroLocal memoria_177(static_cast<unsigned int>(sizeof(bytes_cantidades)));

    if (this->albumes_cargados || this->usuarios == nullptr)
        return false;
    unsigned int *cantidades = nullptr;
    RegistroLocal memoria_178(static_cast<unsigned int>(sizeof(cantidades)));

    try
    {
        {
            RegistroLocal memoria_179(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
            {
                contar_iteracion();
                if (this->usuarios[i].get_cantidad_albumes() != 0)
                    throw "Albumes ya creados";
            }
        }
        Archivo archivo(ruta);
        RegistroLocal memoria_180(static_cast<unsigned int>(sizeof(archivo)));

        if (!archivo.cargar() || archivo.get_cantidad_lineas() % 2 != 0)
            throw "Se requieren dos lineas por album";
        if (this->cantidad_usuarios > static_cast<unsigned int>(-1) / static_cast<unsigned int>(sizeof(unsigned int)))
            throw "Demasiados usuarios";
        cantidades = registrar_reserva(new unsigned int[this->cantidad_usuarios](),
                                       (bytes_cantidades = (this->cantidad_usuarios) * static_cast<unsigned int>(sizeof(unsigned int))));
        {
            RegistroLocal memoria_181(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < archivo.get_cantidad_lineas(); i += 2)
            {
                contar_iteracion();
                unsigned int pos = 0;
                RegistroLocal memoria_182(static_cast<unsigned int>(sizeof(pos)));

                string nombre;
                RegistroLocal memoria_183(static_cast<unsigned int>(sizeof(nombre)));

                if (!tomar_campo(archivo.get_linea(i), pos, nombre, false))
                    throw "Album invalido";
                unsigned int usuario = this->buscar_usuario(nombre);
                RegistroLocal memoria_184(static_cast<unsigned int>(sizeof(usuario)));

                if (usuario >= this->cantidad_usuarios)
                    throw "Propietario inexistente";
                cantidades[usuario]++;
            }
        }
        {
            RegistroLocal memoria_185(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
            {
                contar_iteracion();
                if (!this->usuarios[i].reservar_albumes(cantidades[i]))
                    throw "No se reservaron albumes";
            }
        }
        (registrar_liberacion(cantidades != nullptr ? bytes_cantidades : 0), delete[] cantidades);
        cantidades = nullptr;
        {
            RegistroLocal memoria_186(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < archivo.get_cantidad_lineas(); i += 2)
            {
                contar_iteracion();
                string campos[5];
                RegistroLocal memoria_187(static_cast<unsigned int>(sizeof(campos)));

                unsigned int pos = 0;
                RegistroLocal memoria_188(static_cast<unsigned int>(sizeof(pos)));

                {
                    RegistroLocal memoria_189(1 * static_cast<unsigned int>(sizeof(unsigned char)));
                    for (unsigned char j = 0; j < 5; j++)
                    {
                        contar_iteracion();
                        if (!tomar_campo(archivo.get_linea(i), pos, campos[j], j == 4) || campos[j] == "")
                            throw "Datos de album invalidos";
                    }
                }
                unsigned int usuario = this->buscar_usuario(campos[0]);
                RegistroLocal memoria_190(static_cast<unsigned int>(sizeof(usuario)));

                unsigned int fotos;
                RegistroLocal memoria_191(static_cast<unsigned int>(sizeof(fotos)));

                if (!numero_natural(campos[4], fotos))
                    throw "Cantidad de fotos invalida";
                Usuario &u = this->usuarios[usuario];
                Album album(campos[1], &u, campos[2]);
                if (!album.set_privacidad(campos[3]) || !u.agregar_album(album))
                    throw "Album repetido o invalido";
                Album *destino = u.get_album(u.get_cantidad_albumes() - 1);
                RegistroLocal memoria_192(static_cast<unsigned int>(sizeof(destino)));

                if (!destino->reservar_fotos(fotos))
                    throw "No se reservaron fotos";
                const string &linea = archivo.get_linea(i + 1);
                if (fotos == 0 && linea != "")
                    throw "Album sin fotos con datos extra";
                pos = 0;
                {
                    RegistroLocal memoria_193(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                    for (unsigned int j = 0; j < fotos; j++)
                    {
                        contar_iteracion();
                        string datos[5];
                        RegistroLocal memoria_194(static_cast<unsigned int>(sizeof(datos)));

                        {
                            RegistroLocal memoria_195(1 * static_cast<unsigned int>(sizeof(unsigned char)));
                            for (unsigned char k = 0; k < 5; k++)
                            {
                                contar_iteracion();
                                if (!tomar_campo(linea, pos, datos[k], j + 1 == fotos && k == 4))
                                    throw "Datos de foto incompletos";
                            }
                        }
                        Fecha fecha;
                        RegistroLocal memoria_196(static_cast<unsigned int>(sizeof(fecha)));

                        unsigned int tamano, likes;
                        RegistroLocal memoria_197(static_cast<unsigned int>(sizeof(tamano)) + static_cast<unsigned int>(sizeof(likes)));

                        if (datos[0] == "" || !this->convertir_fecha(datos[1], fecha) ||
                            !numero_natural(datos[2], tamano) || !numero_natural(datos[3], likes))
                            throw "Foto invalida";
                        Foto foto(datos[0], datos[4], fecha, static_cast<unsigned int>(tamano), likes);
                        RegistroLocal memoria_198(static_cast<unsigned int>(sizeof(foto)));

                        if (!destino->agregar_foto(foto))
                            throw "No se pudo agregar foto";
                    }
                }
            }
        }
        this->albumes_cargados = true;
        return true;
    }
    catch (...)
    {
        (registrar_liberacion(cantidades != nullptr ? bytes_cantidades : 0), delete[] cantidades);
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron cargar los albumes. Termine la ejecucion sin guardar.\n";
        }
        return false;
    }
}
bool RedSocial::guardar_albumes(const string &ruta)
{
    RegistroLocal memoria_199(static_cast<unsigned int>(sizeof(this)));

    if (this->error_fatal || !this->albumes_cargados)
        return false;
    try
    {
        ofstream archivo(ruta);
        RegistroLocal memoria_200(static_cast<unsigned int>(sizeof(archivo)));
        contar_llamada(2);
        if (!archivo)
            throw "No se pudo abrir albumes";
        {
            RegistroLocal memoria_201(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < this->cantidad_usuarios; i++)
            {
                contar_iteracion();
                const Usuario &u = this->usuarios[i];
                {
                    RegistroLocal memoria_202(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                    for (unsigned int j = 0; j < u.get_cantidad_albumes(); j++)
                    {
                        contar_iteracion();
                        const Album *a = u.get_album(j);
                        RegistroLocal memoria_203(static_cast<unsigned int>(sizeof(a)));

                        {
                            contar_llamadas(9, 10);
                            archivo << u.get_nombre() << ',' << a->get_nombre() << ',' << a->get_ruta() << ','
                                    << a->get_privacidad() << ',' << a->get_cantidad_fotos() << '\n';
                        }
                        {
                            RegistroLocal memoria_204(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                            for (unsigned int k = 0; k < a->get_cantidad_fotos(); k++)
                            {
                                contar_iteracion();
                                const Foto *f = a->get_foto(k);
                                RegistroLocal memoria_205(static_cast<unsigned int>(sizeof(f)));

                                if (k != 0)
                                {
                                    contar_llamadas(9, 1);
                                    archivo << ',';
                                }
                                {
                                    contar_llamadas(9, 2);
                                    archivo << f->get_ruta() << ',';
                                }
                                escribir_fecha_archivo(archivo, f->get_fecha_captura());
                                {
                                    contar_llamadas(9, 6);
                                    archivo << ',' << f->get_tamano() << ',' << f->get_likes() << ','
                                            << f->get_descripcion();
                                }
                            }
                        }
                        {
                            contar_llamadas(9, 1);
                            archivo << '\n';
                        }
                    }
                }
            }
        }
        (contar_llamada(3), archivo.close());
        if ((contar_llamada(10), archivo.fail()))
            throw "Error al escribir albumes";
        return true;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudieron guardar completamente los albumes.\n";
        }
        return false;
    }
}
void RedSocial::menu_visitar_perfil_amigo(unsigned int pos_usuario)
{
    RegistroLocal memoria_206(static_cast<unsigned int>(sizeof(pos_usuario)) + static_cast<unsigned int>(sizeof(this)));

    try
    {

        if (pos_usuario >= this->cantidad_usuarios)
            return;
        string nombre;
        RegistroLocal memoria_207(static_cast<unsigned int>(sizeof(nombre)));

        {
            contar_llamadas(9, 1);
            cout << "Nombre del amigo: ";
        }
        if (!(contar_llamada(0), getline(cin, nombre)))
            return;
        unsigned int amigo = this->buscar_usuario(nombre);
        RegistroLocal memoria_208(static_cast<unsigned int>(sizeof(amigo)));

        if (!this->relacion(pos_usuario, amigo))
        {
            {
                contar_llamadas(9, 1);
                cout << "No es un amigo registrado.\n";
            }
            return;
        }
        Usuario *visitante = &this->usuarios[pos_usuario];
        RegistroLocal memoria_209(static_cast<unsigned int>(sizeof(visitante)));

        Usuario *propietario = &this->usuarios[amigo];
        RegistroLocal memoria_210(static_cast<unsigned int>(sizeof(propietario)));

        unsigned int visibles = 0;
        RegistroLocal memoria_211(static_cast<unsigned int>(sizeof(visibles)));

        {
            RegistroLocal memoria_212(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < propietario->get_cantidad_albumes(); i++)
            {
                contar_iteracion();
                Album *a = propietario->get_album(i);
                RegistroLocal memoria_213(static_cast<unsigned int>(sizeof(a)));

                if (a->puede_ver(visitante, true))
                {
                    contar_llamadas(9, 4);
                    cout << ++visibles << ". " << a->get_nombre() << '\n';
                }
            }
        }
        if (visibles == 0)
        {
            {
                contar_llamadas(9, 1);
                cout << "No hay albumes disponibles.\n";
            }
            return;
        }
        string entrada;
        RegistroLocal memoria_214(static_cast<unsigned int>(sizeof(entrada)));

        unsigned int opcion;
        RegistroLocal memoria_215(static_cast<unsigned int>(sizeof(opcion)));

        do
        {
            contar_iteracion();
            {
                contar_llamadas(9, 1);
                cout << "Selecciona un album, o 0 para volver: ";
            }
            if (!(contar_llamada(0), getline(cin, entrada)))
                return;
            if (numero_natural(entrada, opcion) && opcion <= visibles)
                break;
            {
                contar_llamadas(9, 1);
                cout << "Opcion invalida.\n";
            }
        } while (true);
        if (opcion == 0)
            return;
        unsigned int contador = 0;
        RegistroLocal memoria_216(static_cast<unsigned int>(sizeof(contador)));

        {
            RegistroLocal memoria_217(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < propietario->get_cantidad_albumes(); i++)
            {
                contar_iteracion();
                Album *a = propietario->get_album(i);
                RegistroLocal memoria_218(static_cast<unsigned int>(sizeof(a)));

                if (a->puede_ver(visitante, true) && ++contador == opcion)
                {
                    a->ver_carrusel(visitante, true);
                    return;
                }
            }
        }
    }
    catch (...)
    {
        this->error_fatal = true;
        {
            contar_llamadas(9, 1);
            cout << "No se pudo completar la operacion.\n";
        }
    }
}

void RedSocial::ordenar_nuevos()
{
    RegistroLocal memoria_219(static_cast<unsigned int>(sizeof(this)));

    {
        RegistroLocal memoria_220(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int ancho = 1; ancho < cantidad_nuevos;)
        {
            contar_iteracion();
            NodoMensaje *actual = nuevos_mensajes;
            RegistroLocal memoria_221(static_cast<unsigned int>(sizeof(actual)));

            NodoMensaje *cabeza = nullptr;
            RegistroLocal memoria_222(static_cast<unsigned int>(sizeof(cabeza)));

            NodoMensaje *cola = nullptr;
            RegistroLocal memoria_223(static_cast<unsigned int>(sizeof(cola)));

            while (actual != nullptr)
            {
                contar_iteracion();
                NodoMensaje *izquierda = actual;
                RegistroLocal memoria_224(static_cast<unsigned int>(sizeof(izquierda)));

                unsigned int ni = 0;
                RegistroLocal memoria_225(static_cast<unsigned int>(sizeof(ni)));

                for (; ni < ancho && actual != nullptr; ni++)
                {
                    contar_iteracion();
                    actual = actual->siguiente;
                }
                NodoMensaje *derecha = actual;
                RegistroLocal memoria_226(static_cast<unsigned int>(sizeof(derecha)));

                unsigned int nd = 0;
                RegistroLocal memoria_227(static_cast<unsigned int>(sizeof(nd)));

                for (; nd < ancho && actual != nullptr; nd++)
                {
                    contar_iteracion();
                    actual = actual->siguiente;
                }
                while (ni != 0 || nd != 0)
                {
                    contar_iteracion();
                    NodoMensaje *elegido;
                    RegistroLocal memoria_228(static_cast<unsigned int>(sizeof(elegido)));

                    if (ni != 0 && (nd == 0 || !mensaje_va_antes(derecha->mensaje, izquierda->mensaje)))
                    {
                        elegido = izquierda;
                        izquierda = izquierda->siguiente;
                        ni--;
                    }
                    else
                    {
                        elegido = derecha;
                        derecha = derecha->siguiente;
                        nd--;
                    }
                    if (cola == nullptr)
                        cabeza = elegido;
                    else
                        cola->siguiente = elegido;
                    cola = elegido;
                }
            }
            if (cola != nullptr)
                cola->siguiente = nullptr;
            nuevos_mensajes = cabeza;
            if (ancho > cantidad_nuevos / 2)
                break;
            ancho *= 2;
        }
    }
}

bool RedSocial::enviar_solicitud(unsigned int emisor, unsigned int receptor)
{
    RegistroLocal memoria_229(static_cast<unsigned int>(sizeof(emisor)) + static_cast<unsigned int>(sizeof(receptor)) + static_cast<unsigned int>(sizeof(this)));

    if (emisor >= cantidad_usuarios || receptor >= cantidad_usuarios || emisor == receptor ||
        relacion(emisor, receptor) || usuarios[receptor].tiene_solicitud(emisor) ||
        usuarios[emisor].tiene_solicitud(receptor))
        return false;
    return usuarios[receptor].agregar_solicitud(emisor);
}
bool RedSocial::aceptar_solicitud(unsigned int receptor, unsigned int posicion)
{
    RegistroLocal memoria_230(static_cast<unsigned int>(sizeof(receptor)) + static_cast<unsigned int>(sizeof(posicion)) + static_cast<unsigned int>(sizeof(this)));

    if (receptor >= cantidad_usuarios || posicion >= usuarios[receptor].get_cantidad_solicitudes())
        return false;
    unsigned int emisor = usuarios[receptor].get_solicitud(posicion);
    RegistroLocal memoria_231(static_cast<unsigned int>(sizeof(emisor)));

    if (!agregar_amistad(emisor, receptor))
        return false;
    return usuarios[receptor].retirar_solicitud(posicion);
}
void RedSocial::menu_solicitudes(unsigned int usuario)
{
    RegistroLocal memoria_232(static_cast<unsigned int>(sizeof(usuario)) + static_cast<unsigned int>(sizeof(this)));

    unsigned int opcion;
    RegistroLocal memoria_233(static_cast<unsigned int>(sizeof(opcion)));

    if (!pedir_numero("1. Enviar solicitud\n2. Aceptar recibida\n3. Rechazar recibida\n0. Volver\nOpcion: ",
                      0, 3, opcion) ||
        opcion == 0)
        return;
    if (opcion == 1)
    {
        string nombre;
        RegistroLocal memoria_234(static_cast<unsigned int>(sizeof(nombre)));

        {
            contar_llamadas(9, 1);
            cout << "Nombre del destinatario: ";
        }
        if (!(contar_llamada(0), getline(cin, nombre)))
            return;
        if (enviar_solicitud(usuario, buscar_usuario(nombre)))
        {
            contar_llamadas(9, 1);
            cout << "Solicitud enviada.\n";
        }
        else
        {
            contar_llamadas(9, 1);
            cout << "No se puede: usuario inexistente, propio, amistad o solicitud pendiente.\n";
        }
        return;
    }
    unsigned int cantidad = usuarios[usuario].get_cantidad_solicitudes();
    RegistroLocal memoria_235(static_cast<unsigned int>(sizeof(cantidad)));

    if (cantidad == 0)
    {
        {
            contar_llamadas(9, 1);
            cout << "No tienes solicitudes recibidas.\n";
        }
        return;
    }
    {
        RegistroLocal memoria_236(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < cantidad; i++)
        {
            contar_iteracion();
            {
                contar_llamadas(9, 4);
                cout << i + 1 << ". " << usuarios[usuarios[usuario].get_solicitud(i)].get_nombre() << '\n';
            }
        }
    }
    unsigned int posicion;
    RegistroLocal memoria_237(static_cast<unsigned int>(sizeof(posicion)));

    if (!pedir_numero("Numero (0 para volver): ", 0, cantidad, posicion) || posicion == 0)
        return;
    bool correcto = opcion == 2 ? aceptar_solicitud(usuario, posicion - 1)
                                : usuarios[usuario].retirar_solicitud(posicion - 1);
    RegistroLocal memoria_238(static_cast<unsigned int>(sizeof(correcto)));

    {
        contar_llamadas(9, 1);
        cout << (correcto ? "Solicitud procesada.\n" : "No se pudo procesar.\n");
    }
}
void RedSocial::menu_sugerencias(unsigned int usuario)
{
    RegistroLocal memoria_239(static_cast<unsigned int>(sizeof(usuario)) + static_cast<unsigned int>(sizeof(this)));

    unsigned int k;
    RegistroLocal memoria_240(static_cast<unsigned int>(sizeof(k)));

    if (!pedir_numero("Cantidad de sugerencias (1 a 20): ", 1, 20, k))
        return;
    unsigned int cantidad = 0;
    RegistroLocal memoria_241(static_cast<unsigned int>(sizeof(cantidad)));

    Sugerencia *lista = hallar_sugerencias(usuario, k, cantidad);
    RegistroLocal memoria_242(static_cast<unsigned int>(sizeof(lista)));

    try
    {
        if (cantidad == 0)
        {
            contar_llamadas(9, 1);
            cout << "No hay sugerencias con amigos en comun.\n";
        }
        else
        {
            {
                RegistroLocal memoria_243(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                for (unsigned int i = 0; i < cantidad; i++)
                {
                    contar_iteracion();
                    {
                        contar_llamadas(9, 6);
                        cout << i + 1 << ". " << lista[i].get_usuario()->get_nombre()
                             << " | Amigos en comun: " << lista[i].get_amigos_comunes() << '\n';
                    }
                }
            }
            unsigned int opcion;
            RegistroLocal memoria_244(static_cast<unsigned int>(sizeof(opcion)));

            if (pedir_numero("Enviar solicitud a (0 para volver): ", 0, cantidad, opcion) && opcion != 0)
            {
                unsigned int destino = static_cast<unsigned int>(lista[opcion - 1].get_usuario() - usuarios);
                RegistroLocal memoria_245(static_cast<unsigned int>(sizeof(destino)));

                {
                    contar_llamadas(9, 1);
                    cout << (enviar_solicitud(usuario, destino)
                                 ? "Solicitud enviada.\n"
                                 : "Ya existe amistad o solicitud pendiente.\n");
                }
            }
        }
    }
    catch (...)
    {
        (registrar_liberacion(lista != nullptr ? k * static_cast<unsigned int>(sizeof(Sugerencia)) : 0), delete[] lista);
        throw;
    }
    (registrar_liberacion(lista != nullptr ? k * static_cast<unsigned int>(sizeof(Sugerencia)) : 0), delete[] lista);
}
void RedSocial::mostrar_perfiles(unsigned int usuario, unsigned int *anteriores, unsigned int &cantidad_anteriores,
                                 bool renovar)
{
    if (!generador_iniciado)
    {
        random_device semilla;
        RegistroLocal memoria_semilla(static_cast<unsigned int>(sizeof(semilla)));
        contar_llamada(6);
        contar_llamada(8);
        generador.seed(semilla());
        generador_iniciado = true;
    }

    RegistroLocal memoria_246(static_cast<unsigned int>(sizeof(usuario)) + static_cast<unsigned int>(sizeof(anteriores)) + static_cast<unsigned int>(sizeof(renovar)) + static_cast<unsigned int>(sizeof(this)));

    unsigned int bytes_amigos = 0;
    RegistroLocal memoria_247(static_cast<unsigned int>(sizeof(bytes_amigos)));

    unsigned int bytes_pesos = 0;
    RegistroLocal memoria_248(static_cast<unsigned int>(sizeof(bytes_pesos)));

    unsigned int cantidad = 0;
    RegistroLocal memoria_249(static_cast<unsigned int>(sizeof(cantidad)));

    {
        RegistroLocal memoria_250(1 * static_cast<unsigned int>(sizeof(unsigned int)));
        for (unsigned int i = 0; i < cantidad_usuarios; i++)
        {
            contar_iteracion();
            if (relacion(usuario, i))
                cantidad++;
        }
    }
    if (cantidad == 0)
    {
        {
            contar_llamadas(9, 1);
            cout << "No tienes amigos para mostrar perfiles.\n";
        }
        return;
    }
    unsigned int *amigos = nullptr;
    RegistroLocal memoria_251(static_cast<unsigned int>(sizeof(amigos)));

    unsigned char *pesos = nullptr;
    RegistroLocal memoria_252(static_cast<unsigned int>(sizeof(pesos)));

    try
    {
        amigos = registrar_reserva(new unsigned int[cantidad], (bytes_amigos = (cantidad) * static_cast<unsigned int>(sizeof(unsigned int))));
        pesos = registrar_reserva(new unsigned char[cantidad],
                                  (bytes_pesos = (cantidad) * static_cast<unsigned int>(sizeof(unsigned char))));
        unsigned int j = 0;
        RegistroLocal memoria_253(static_cast<unsigned int>(sizeof(j)));

        {
            RegistroLocal memoria_254(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < cantidad_usuarios; i++)
            {
                contar_iteracion();
                if (relacion(usuario, i))
                    amigos[j++] = i;
            }
        }
        unsigned int suma = 0;
        RegistroLocal memoria_255(static_cast<unsigned int>(sizeof(suma)));

        {
            RegistroLocal memoria_256(1 * static_cast<unsigned int>(sizeof(unsigned int)));
            for (unsigned int i = 0; i < cantidad; i++)
            {
                contar_iteracion();
                bool excluido = false;
                RegistroLocal memoria_257(static_cast<unsigned int>(sizeof(excluido)));

                if (renovar)
                {
                    RegistroLocal memoria_258(1 * static_cast<unsigned int>(sizeof(unsigned int)));
                    for (unsigned int k = 0; k < cantidad_anteriores; k++)
                    {
                        contar_iteracion();
                        if (amigos[i] == anteriores[k])
                            excluido = true;
                    }
                }
                unsigned int comunes = excluido ? 0 : cant_amigos_comunes(amigos[i], amigos, cantidad);
                RegistroLocal memoria_259(static_cast<unsigned int>(sizeof(comunes)));

                pesos[i] = excluido ? 0 : comunes <= 10 ? 2 : comunes <= 20 ? 5 : 7;
                if (suma > static_cast<unsigned int>(-1) - pesos[i])
                    throw "Demasiados pesos";
                suma += pesos[i];
            }
        }
        cantidad_anteriores = 0;
        while (cantidad_anteriores < 3 && suma > 0)
        {
            contar_iteracion();
            uniform_int_distribution<unsigned int> distribucion(0, suma - 1);
            RegistroLocal memoria_260(static_cast<unsigned int>(sizeof(distribucion)));

            unsigned int boleto = (contar_llamada(7), distribucion(generador));
            RegistroLocal memoria_261(static_cast<unsigned int>(sizeof(boleto)));

            unsigned int seleccionado = 0;
            RegistroLocal memoria_262(static_cast<unsigned int>(sizeof(seleccionado)));

            while (boleto >= pesos[seleccionado])
            {
                contar_iteracion();
                boleto -= pesos[seleccionado++];
            }
            unsigned int indice = amigos[seleccionado];
            RegistroLocal memoria_263(static_cast<unsigned int>(sizeof(indice)));

            anteriores[cantidad_anteriores++] = indice;
            {
                contar_llamadas(9, 4);
                cout << usuarios[indice].get_nombre() << " | Foto: " << usuarios[indice].get_ruta_perfil()
                     << '\n';
            }
            suma -= pesos[seleccionado];
            pesos[seleccionado] = 0;
        }
        if (cantidad_anteriores < 3)
        {
            contar_llamadas(9, 1);
            cout << "No hay tres amigos disponibles sin repetir perfiles.\n";
        }
    }
    catch (...)
    {
        (registrar_liberacion(amigos != nullptr ? bytes_amigos : 0), delete[] amigos);
        (registrar_liberacion(pesos != nullptr ? bytes_pesos : 0), delete[] pesos);
        throw;
    }
    (registrar_liberacion(amigos != nullptr ? bytes_amigos : 0), delete[] amigos);
    (registrar_liberacion(pesos != nullptr ? bytes_pesos : 0), delete[] pesos);
}
