#include "redsocial.h"

#include <fstream>
#include <iostream>

using namespace std;

RedSocial::RedSocial()
{
    this->usuarios = nullptr;
    this->cantidad_usuarios = 0;

    this->relaciones = nullptr;
    this->bytes_relaciones = 0;
    this->sobrantes = 0;

    this->mensajes = nullptr;
    this->cantidad_mensajes = 0;
    this->capacidad_mensajes = 0;
    this->mensajes_cargados = false;
}

RedSocial::RedSocial(int cantidad_usuarios)
{
    this->usuarios = nullptr;
    this->cantidad_usuarios = 0;

    this->relaciones = nullptr;
    this->bytes_relaciones = 0;
    this->sobrantes = 0;

    this->mensajes = nullptr;
    this->cantidad_mensajes = 0;
    this->capacidad_mensajes = 0;
    this->mensajes_cargados = false;

    if (cantidad_usuarios <= 0)
    {
        return;
    }

    this->cantidad_usuarios = cantidad_usuarios;
    this->usuarios = new Usuario[cantidad_usuarios];

    long long bits = 1LL * cantidad_usuarios *
                     (cantidad_usuarios - 1) / 2;

    this->bytes_relaciones = (bits + 7) / 8;
    this->sobrantes = this->bytes_relaciones * 8LL - bits;

    if (this->bytes_relaciones > 0)
    {
        this->relaciones =
            new unsigned char[this->bytes_relaciones]();
    }
}

RedSocial::~RedSocial()
{
    delete[] this->mensajes;
    delete[] this->relaciones;
    delete[] this->usuarios;
}

int RedSocial::get_cantidad_usuarios() const
{
    return this->cantidad_usuarios;
}

int RedSocial::get_cantidad_mensajes() const
{
    return this->cantidad_mensajes;
}

Usuario* RedSocial::get_usuario(int posicion)
{
    if (posicion < 0 || posicion >= this->cantidad_usuarios)
    {
        return nullptr;
    }

    return &this->usuarios[posicion];
}

int RedSocial::buscar_usuario(const string& nombre) const
{
    int izquierda = 0;
    int derecha = this->cantidad_usuarios - 1;

    while (izquierda <= derecha)
    {
        int mitad = izquierda + (derecha - izquierda) / 2;
        string actual = this->usuarios[mitad].get_nombre();

        if (actual == nombre)
        {
            return mitad;
        }

        if (actual < nombre)
        {
            izquierda = mitad + 1;
        }
        else
        {
            derecha = mitad - 1;
        }
    }

    return -1;
}

bool RedSocial::leer_bit(int posicion) const
{
    int real = posicion + this->sobrantes;
    int byte = real / 8;
    int bit = 7 - real % 8;

    return ((this->relaciones[byte] >> bit) & 1u) != 0;
}

int RedSocial::posicion_relacion(int pos1, int pos2) const
{
    if (pos1 > pos2)
    {
        int auxiliar = pos1;
        pos1 = pos2;
        pos2 = auxiliar;
    }

    long long inicio = 1LL * pos1 *
                       (2LL * this->cantidad_usuarios - pos1 - 1) / 2;

    return inicio + pos2 - pos1 - 1;
}

bool RedSocial::relacion(int pos1, int pos2) const
{
    if (pos1 < 0 || pos2 < 0 ||
        pos1 >= this->cantidad_usuarios ||
        pos2 >= this->cantidad_usuarios ||
        pos1 == pos2)
    {
        return false;
    }

    return this->leer_bit(this->posicion_relacion(pos1, pos2));
}

bool RedSocial::agregar_amistad(int pos1, int pos2)
{
    if (pos1 < 0 || pos2 < 0 ||
        pos1 >= this->cantidad_usuarios ||
        pos2 >= this->cantidad_usuarios ||
        pos1 == pos2)
    {
        return false;
    }

    int real = this->posicion_relacion(pos1, pos2) + this->sobrantes;
    int byte = real / 8;
    int bit = 7 - real % 8;

    this->relaciones[byte] |= (1u << bit);

    return true;
}

int RedSocial::cant_amigos_comunes(
    int candidato,
    const int* indices_amigos,
    int cantidad_amigos
    ) const
{
    int comunes = 0;

    for (int i = 0; i < cantidad_amigos; i++)
    {
        if (this->relacion(candidato, indices_amigos[i]))
        {
            comunes++;
        }
    }

    return comunes;
}

Sugerencia* RedSocial::hallar_sugerencias(
    int pos_usuario,
    int k,
    int& cantidad
    )
{
    cantidad = 0;

    if (pos_usuario < 0 ||
        pos_usuario >= this->cantidad_usuarios ||
        k < 1 || k > 20)
    {
        return nullptr;
    }

    int cantidad_amigos = 0;

    for (int i = 0; i < this->cantidad_usuarios; i++)
    {
        if (this->relacion(pos_usuario, i))
        {
            cantidad_amigos++;
        }
    }

    int posibles = this->cantidad_usuarios - cantidad_amigos - 1;

    if (cantidad_amigos == 0 || posibles == 0)
    {
        return nullptr;
    }

    if (k > posibles)
    {
        k = posibles;
    }

    int* indices_amigos = new int[cantidad_amigos];
    int guardados = 0;

    for (int i = 0; i < this->cantidad_usuarios; i++)
    {
        if (this->relacion(pos_usuario, i))
        {
            indices_amigos[guardados] = i;
            guardados++;
        }
    }

    Sugerencia* resultado = new Sugerencia[k];

    for (int candidato = 0;
         candidato < this->cantidad_usuarios;
         candidato++)
    {
        if (candidato == pos_usuario ||
            this->relacion(pos_usuario, candidato))
        {
            continue;
        }

        int comunes = this->cant_amigos_comunes(
            candidato,
            indices_amigos,
            cantidad_amigos
            );

        if (comunes == 0)
        {
            continue;
        }

        if (cantidad == k &&
            comunes <= resultado[k - 1].get_amigos_comunes())
        {
            continue;
        }

        int posicion;

        if (cantidad < k)
        {
            posicion = cantidad;
            cantidad++;
        }
        else
        {
            posicion = k - 1;
        }

        while (posicion > 0 &&
               comunes > resultado[posicion - 1].get_amigos_comunes())
        {
            resultado[posicion] = resultado[posicion - 1];
            posicion--;
        }

        resultado[posicion].set_usuario(&this->usuarios[candidato]);
        resultado[posicion].set_amigos_comunes(comunes);
    }

    delete[] indices_amigos;

    if (cantidad == 0)
    {
        delete[] resultado;
        return nullptr;
    }

    return resultado;
}

bool RedSocial::convertir_entero(
    const string& texto,
    int& numero
    ) const
{
    numero = 0;

    if (texto.empty() || texto.size() > 9)
    {
        return false;
    }

    for (int i = 0; i < int(texto.size()); i++)
    {
        if (texto[i] < '0' || texto[i] > '9')
        {
            return false;
        }

        numero = numero * 10 + texto[i] - '0';
    }

    return true;
}

bool RedSocial::convertir_fecha(
    const string& texto,
    Fecha& fecha
    ) const
{
    if (texto.size() != 10 ||
        texto[4] != '-' || texto[7] != '-')
    {
        return false;
    }

    int anio;
    int mes;
    int dia;

    if (!this->convertir_entero(texto.substr(0, 4), anio) ||
        !this->convertir_entero(texto.substr(5, 2), mes) ||
        !this->convertir_entero(texto.substr(8, 2), dia))
    {
        return false;
    }

    return fecha.set_fecha(dia, mes, anio);
}

bool RedSocial::pedir_fecha(
    const string& texto,
    Fecha& fecha
    ) const
{
    string entrada;

    while (true)
    {
        cout << texto << " (AAAA-MM-DD): ";

        if (!getline(cin, entrada))
        {
            return false;
        }

        if (this->convertir_fecha(entrada, fecha))
        {
            return true;
        }

        cout << "Fecha invalida. Intenta nuevamente.\n";
    }
}

bool RedSocial::fecha_anterior(
    const Fecha& primera,
    const Fecha& segunda
    ) const
{
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

bool RedSocial::cargar_mensajes(const string& ruta)
{
    if (this->mensajes_cargados || this->cantidad_mensajes > 0)
    {
        cout << "La carga inicial de mensajes ya no esta disponible.\n";
        return false;
    }

    ifstream archivo(ruta);

    if (!archivo.is_open())
    {
        cout << "No se pudo abrir el archivo de mensajes.\n";
        return false;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea))
    {
        if (!linea.empty() && linea.back() == '\r')
        {
            linea.pop_back();
        }

        if (!linea.empty())
        {
            total++;
        }
    }

    if (archivo.bad())
    {
        cout << "Error al leer el archivo.\n";
        return false;
    }

    archivo.clear();
    archivo.seekg(0);

    if (!archivo)
    {
        cout << "No se pudo volver al inicio del archivo.\n";
        return false;
    }

    Mensaje* nuevos = nullptr;

    if (total > 0)
    {
        nuevos = new Mensaje[total];
    }

    int cantidad = 0;
    int numero_linea = 0;

    while (getline(archivo, linea))
    {
        numero_linea++;

        if (!linea.empty() && linea.back() == '\r')
        {
            linea.pop_back();
        }

        if (linea.empty())
        {
            continue;
        }

        string campos[4];
        string::size_type inicio = 0;
        bool formato_valido = true;

        for (int i = 0; i < 4; i++)
        {
            string::size_type coma = linea.find(',', inicio);

            if (coma == string::npos)
            {
                formato_valido = false;
                break;
            }

            campos[i] = linea.substr(inicio, coma - inicio);
            inicio = coma + 1;
        }

        if (!formato_valido)
        {
            cout << "Formato incorrecto en la linea "
                 << numero_linea << ".\n";

            delete[] nuevos;
            return false;
        }

        string contenido = linea.substr(inicio);

        int pos_receptor = this->buscar_usuario(campos[0]);
        int pos_emisor = this->buscar_usuario(campos[1]);

        Fecha fecha;

        if (pos_receptor == -1 || pos_emisor == -1 ||
            !this->convertir_fecha(campos[2], fecha) ||
            (campos[3] != "0" && campos[3] != "1") ||
            contenido.empty() || contenido.size() > 512)
        {
            cout << "Datos invalidos en la linea "
                 << numero_linea << ".\n";

            delete[] nuevos;
            return false;
        }

        if (cantidad >= total)
        {
            cout << "El archivo cambio durante la carga.\n";

            delete[] nuevos;
            return false;
        }

        nuevos[cantidad].set_datos(
            contenido,
            &this->usuarios[pos_emisor],
            &this->usuarios[pos_receptor],
            fecha,
            campos[3] == "1"
            );

        cantidad++;
    }

    if (archivo.bad())
    {
        cout << "Error al leer el archivo.\n";

        delete[] nuevos;
        return false;
    }

    delete[] this->mensajes;

    this->mensajes = nuevos;
    this->cantidad_mensajes = cantidad;
    this->capacidad_mensajes = total;
    this->mensajes_cargados = true;

    this->ordenar_mensajes();

    return true;
}

bool RedSocial::mensaje_va_antes(
    const Mensaje& primero,
    const Mensaje& segundo
    ) const
{
    string receptor1 = primero.get_receptor()->get_nombre();
    string receptor2 = segundo.get_receptor()->get_nombre();

    if (receptor1 != receptor2)
    {
        return receptor1 < receptor2;
    }

    return this->fecha_anterior(
        segundo.get_fecha(),
        primero.get_fecha()
        );
}

void RedSocial::ordenar_mensajes()
{
    if (this->cantidad_mensajes < 2)
    {
        return;
    }

    Mensaje* auxiliar = new Mensaje[this->cantidad_mensajes];

    this->merge_sort_mensajes(
        auxiliar,
        0,
        this->cantidad_mensajes
        );

    delete[] auxiliar;
}

void RedSocial::merge_sort_mensajes(
    Mensaje* auxiliar,
    int inicio,
    int fin
    )
{
    if (fin - inicio < 2)
    {
        return;
    }

    int mitad = inicio + (fin - inicio) / 2;

    this->merge_sort_mensajes(auxiliar, inicio, mitad);
    this->merge_sort_mensajes(auxiliar, mitad, fin);

    if (!this->mensaje_va_antes(
            this->mensajes[mitad],
            this->mensajes[mitad - 1]))
    {
        return;
    }

    this->mezclar_mensajes(auxiliar, inicio, mitad, fin);
}

void RedSocial::mezclar_mensajes(
    Mensaje* auxiliar,
    int inicio,
    int mitad,
    int fin
    )
{
    int izquierda = inicio;
    int derecha = mitad;
    int destino = inicio;

    while (izquierda < mitad && derecha < fin)
    {
        if (this->mensaje_va_antes(
                this->mensajes[derecha],
                this->mensajes[izquierda]))
        {
            auxiliar[destino] = this->mensajes[derecha];
            derecha++;
        }
        else
        {
            auxiliar[destino] = this->mensajes[izquierda];
            izquierda++;
        }

        destino++;
    }

    while (izquierda < mitad)
    {
        auxiliar[destino] = this->mensajes[izquierda];
        izquierda++;
        destino++;
    }

    while (derecha < fin)
    {
        auxiliar[destino] = this->mensajes[derecha];
        derecha++;
        destino++;
    }

    for (int i = inicio; i < fin; i++)
    {
        this->mensajes[i] = auxiliar[i];
    }
}

void RedSocial::ampliar_mensajes()
{
    int nueva_capacidad;

    if (this->capacidad_mensajes == 0)
    {
        nueva_capacidad = 1;
    }
    else
    {
        nueva_capacidad = this->capacidad_mensajes +
                          this->capacidad_mensajes / 2 + 1;
    }

    Mensaje* nuevos = new Mensaje[nueva_capacidad];

    for (int i = 0; i < this->cantidad_mensajes; i++)
    {
        nuevos[i] = this->mensajes[i];
    }

    delete[] this->mensajes;

    this->mensajes = nuevos;
    this->capacidad_mensajes = nueva_capacidad;
}

bool RedSocial::enviar_mensaje(
    int pos_emisor,
    int pos_receptor,
    const string& contenido,
    const Fecha& fecha
    )
{
    if (!this->relacion(pos_emisor, pos_receptor))
    {
        return false;
    }

    if (contenido.empty() || contenido.size() > 512)
    {
        return false;
    }

    string receptor = this->usuarios[pos_receptor].get_nombre();

    int izquierda = 0;
    int derecha = this->cantidad_mensajes;

    while (izquierda < derecha)
    {
        int mitad = izquierda + (derecha - izquierda) / 2;

        string receptor_actual =
            this->mensajes[mitad].get_receptor()->get_nombre();

        bool va_antes = receptor_actual < receptor;

        if (receptor_actual == receptor)
        {
            va_antes = this->fecha_anterior(
                fecha,
                this->mensajes[mitad].get_fecha()
                );
        }

        if (va_antes)
        {
            izquierda = mitad + 1;
        }
        else
        {
            derecha = mitad;
        }
    }

    int posicion = izquierda;

    if (this->cantidad_mensajes == this->capacidad_mensajes)
    {
        this->ampliar_mensajes();
    }

    for (int i = this->cantidad_mensajes; i > posicion; i--)
    {
        this->mensajes[i] = this->mensajes[i - 1];
    }

    this->mensajes[posicion].set_datos(
        contenido,
        &this->usuarios[pos_emisor],
        &this->usuarios[pos_receptor],
        fecha,
        false
        );

    this->cantidad_mensajes++;

    return true;
}

void RedSocial::menu_enviar_mensaje(int pos_emisor)
{
    if (pos_emisor < 0 || pos_emisor >= this->cantidad_usuarios)
    {
        cout << "Usuario invalido.\n";
        return;
    }

    int cantidad_amigos = 0;

    cout << "A que amigo quieres enviar un mensaje?\n";

    for (int i = 0; i < this->cantidad_usuarios; i++)
    {
        if (this->relacion(pos_emisor, i))
        {
            cantidad_amigos++;

            cout << cantidad_amigos << ". "
                 << this->usuarios[i].get_nombre() << '\n';
        }
    }

    if (cantidad_amigos == 0)
    {
        cout << "No tienes amigos para enviar mensajes.\n";
        return;
    }

    string entrada;
    int opcion = 0;

    while (true)
    {
        cout << "Selecciona un numero entre 1 y "
             << cantidad_amigos << ": ";

        if (!getline(cin, entrada))
        {
            return;
        }

        if (this->convertir_entero(entrada, opcion) &&
            opcion >= 1 && opcion <= cantidad_amigos)
        {
            break;
        }

        cout << "Opcion invalida. Intenta nuevamente.\n";
    }

    int pos_receptor = -1;
    int contador = 0;

    for (int i = 0; i < this->cantidad_usuarios; i++)
    {
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

    string contenido;

    while (true)
    {
        cout << "Escribe el mensaje, maximo 512 caracteres:\n";

        if (!getline(cin, contenido))
        {
            return;
        }

        if (!contenido.empty() && contenido.size() <= 512)
        {
            break;
        }

        cout << "El mensaje debe tener entre 1 y 512 caracteres.\n";
    }

    Fecha fecha;

    if (!this->pedir_fecha("Fecha de envio", fecha))
    {
        return;
    }

    if (this->enviar_mensaje(
            pos_emisor,
            pos_receptor,
            contenido,
            fecha))
    {
        cout << "Mensaje enviado.\n";
    }
    else
    {
        cout << "No se pudo enviar el mensaje.\n";
    }
}

void RedSocial::limites_mensajes(
    int pos_usuario,
    int& inicio,
    int& fin
    ) const
{
    inicio = 0;
    fin = 0;

    if (pos_usuario < 0 || pos_usuario >= this->cantidad_usuarios)
    {
        return;
    }

    string nombre = this->usuarios[pos_usuario].get_nombre();

    int izquierda = 0;
    int derecha = this->cantidad_mensajes;

    while (izquierda < derecha)
    {
        int mitad = izquierda + (derecha - izquierda) / 2;

        string receptor =
            this->mensajes[mitad].get_receptor()->get_nombre();

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
        int mitad = izquierda + (derecha - izquierda) / 2;

        string receptor =
            this->mensajes[mitad].get_receptor()->get_nombre();

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

void RedSocial::mostrar_mensaje(int posicion) const
{
    const Mensaje& mensaje = this->mensajes[posicion];

    cout << "\nEmisor: "
         << mensaje.get_emisor()->get_nombre() << '\n';

    cout << "Fecha: "
         << mensaje.get_fecha().get_fecha() << '\n';

    cout << "Estado: "
         << (mensaje.get_leido() ? "Leido" : "No leido") << '\n';

    cout << "Contenido: "
         << mensaje.get_contenido() << '\n';
}

void RedSocial::leer_mensajes_nuevos(int pos_usuario)
{
    if (pos_usuario < 0 || pos_usuario >= this->cantidad_usuarios)
    {
        cout << "Usuario invalido.\n";
        return;
    }

    int inicio;
    int fin;

    this->limites_mensajes(pos_usuario, inicio, fin);

    int mostrados = 0;

    for (int i = inicio; i < fin; i++)
    {
        if (!this->mensajes[i].get_leido())
        {
            this->mostrar_mensaje(i);
            this->mensajes[i].marcar_leido();
            mostrados++;
        }
    }

    if (mostrados == 0)
    {
        cout << "No tienes mensajes nuevos.\n";
    }
    else
    {
        cout << "\nLos mensajes mostrados quedaron marcados como leidos.\n";
    }
}

void RedSocial::buscar_mensajes_por_fecha(
    int pos_usuario,
    const Fecha& fecha_inicial,
    const Fecha& fecha_final
    ) const
{
    if (pos_usuario < 0 || pos_usuario >= this->cantidad_usuarios)
    {
        cout << "Usuario invalido.\n";
        return;
    }

    if (this->fecha_anterior(fecha_final, fecha_inicial))
    {
        cout << "La fecha inicial no puede ser posterior a la final.\n";
        return;
    }

    int inicio;
    int fin;

    this->limites_mensajes(pos_usuario, inicio, fin);

    int izquierda = inicio;
    int derecha = fin;

    while (izquierda < derecha)
    {
        int mitad = izquierda + (derecha - izquierda) / 2;

        if (this->fecha_anterior(
                fecha_final,
                this->mensajes[mitad].get_fecha()))
        {
            izquierda = mitad + 1;
        }
        else
        {
            derecha = mitad;
        }
    }

    int mostrados = 0;

    for (int i = izquierda; i < fin; i++)
    {
        if (this->fecha_anterior(
                this->mensajes[i].get_fecha(),
                fecha_inicial))
        {
            break;
        }

        this->mostrar_mensaje(i);
        mostrados++;
    }

    if (mostrados == 0)
    {
        cout << "No tienes mensajes en ese rango de fechas.\n";
    }
}

void RedSocial::menu_buscar_mensajes_por_fecha(
    int pos_usuario
    ) const
{
    if (pos_usuario < 0 || pos_usuario >= this->cantidad_usuarios)
    {
        cout << "Usuario invalido.\n";
        return;
    }

    Fecha fecha_inicial;
    Fecha fecha_final;

    if (!this->pedir_fecha("Fecha inicial", fecha_inicial))
    {
        return;
    }

    while (true)
    {
        if (!this->pedir_fecha("Fecha final", fecha_final))
        {
            return;
        }

        if (!this->fecha_anterior(fecha_final, fecha_inicial))
        {
            break;
        }

        cout << "La fecha final no puede ser anterior a la inicial.\n";
    }

    this->buscar_mensajes_por_fecha(
        pos_usuario,
        fecha_inicial,
        fecha_final
        );


    int RedSocial::buscar_usuario_en(
        const Usuario* lista,
        int cantidad,
        const string& nombre) const
    {
        int izquierda = 0;
        int derecha = cantidad - 1;

        while (izquierda <= derecha) {

            int mitad = izquierda + (derecha - izquierda) / 2;

            string actual = lista[mitad].get_nombre();

            if (actual == nombre)
                return mitad;

            if (actual < nombre)
                izquierda = mitad + 1;
            else
                derecha = mitad - 1;
        }

        return -1;
    }


    bool RedSocial::cargar_usuarios(const string& ruta)
    {
        if (this->usuarios != nullptr ||
            this->cantidad_mensajes > 0 ||
            this->mensajes_cargados)
            return false;

        Recursos archivo(ruta);

        if (!archivo.cargar())
            return false;

        int total_lineas = archivo.get_cantidad_lineas();

        if (total_lineas == 0 || total_lineas % 3 != 0)
            return false;

        int total = total_lineas / 3;

        Usuario* nuevos = new Usuario[total];

        for (int i = 0; i < total; i++) {

            string linea = archivo.get_linea(i * 3);
            string campos[8];

            string::size_type inicio = 0;
            bool valido = true;

            for (int j = 0; j < 7; j++) {

                string::size_type fin = linea.find('|', inicio);

                if (fin == string::npos) {
                    valido = false;
                    break;
                }

                campos[j] = linea.substr(inicio, fin - inicio);
                inicio = fin + 1;
            }

            if (!valido) {
                delete[] nuevos;
                return false;
            }

            campos[7] = linea.substr(inicio);

            if (campos[7].find('|') != string::npos) {
                delete[] nuevos;
                return false;
            }

            for (int j = 0; j < 8; j++) {
                if (campos[j].empty()) {
                    valido = false;
                    break;
                }
            }

            Fecha nacimiento;
            Fecha inscripcion;

            if (!valido ||
                !this->convertir_fecha(campos[5], nacimiento) ||
                !this->convertir_fecha(campos[6], inscripcion)) {

                delete[] nuevos;
                return false;
            }

            if (i > 0 &&
                nuevos[i - 1].get_nombre() >= campos[0]) {

                delete[] nuevos;
                return false;
            }

            nuevos[i].set_datos(
                campos[0],
                campos[1],
                campos[2],
                campos[3],
                campos[4],
                nacimiento,
                inscripcion
                );
        }

        long long bits = 1LL * total * (total - 1) / 2;

        int bytes = (bits + 7) / 8;
        int sobrantes = bytes * 8LL - bits;

        unsigned char* nuevas_relaciones = nullptr;

        if (bytes > 0)
            nuevas_relaciones = new unsigned char[bytes]();

        for (int i = 0; i < total; i++) {

            string lista = archivo.get_linea(i * 3 + 1);

            if (lista.empty())
                continue;

            string::size_type inicio = 0;

            while (true) {

                string::size_type fin = lista.find('|', inicio);

                string nombre;

                if (fin == string::npos)
                    nombre = lista.substr(inicio);
                else
                    nombre = lista.substr(inicio, fin - inicio);

                int amigo = this->buscar_usuario_en(
                    nuevos, total, nombre
                    );

                if (amigo == -1 || amigo == i) {
                    delete[] nuevas_relaciones;
                    delete[] nuevos;
                    return false;
                }

                int pos1 = i;
                int pos2 = amigo;

                if (pos1 > pos2) {
                    int auxiliar = pos1;
                    pos1 = pos2;
                    pos2 = auxiliar;
                }

                long long posicion =
                    1LL * pos1 * (2LL * total - pos1 - 1) / 2
                    + pos2 - pos1 - 1;

                long long real = posicion + sobrantes;

                int byte = real / 8;
                int bit = 7 - real % 8;

                nuevas_relaciones[byte] |= (1u << bit);

                if (fin == string::npos)
                    break;

                inicio = fin + 1;
            }
        }

        this->archivo_usuarios = archivo;

        this->usuarios = nuevos;
        this->cantidad_usuarios = total;

        this->relaciones = nuevas_relaciones;
        this->bytes_relaciones = bytes;
        this->sobrantes = sobrantes;

        return true;
    }


    bool RedSocial::guardar_usuarios()
    {
        if (this->usuarios == nullptr)
            return false;

        if (this->archivo_usuarios.get_cantidad_lineas()
            != this->cantidad_usuarios * 3)
            return false;

        for (int i = 0; i < this->cantidad_usuarios; i++) {

            string amigos = "";

            for (int j = 0; j < this->cantidad_usuarios; j++) {

                if (this->relacion(i, j)) {

                    if (!amigos.empty())
                        amigos += "|";

                    amigos += this->usuarios[j].get_nombre();
                }
            }

            if (!this->archivo_usuarios.modificar_linea(
                    i * 3 + 1, amigos))
                return false;
        }

        return this->archivo_usuarios.guardar();
    }


    void RedSocial::menu_visitar_perfil_amigo(int pos_usuario)
    {
        Usuario* visitante = this->get_usuario(pos_usuario);

        if (visitante == nullptr) {
            cout << "Usuario invalido.\n";
            return;
        }

        string nombre;

        cout << "\nNombre del amigo a visitar: ";

        if (!getline(cin, nombre))
            return;

        int pos_amigo = this->buscar_usuario(nombre);

        if (pos_amigo == -1) {
            cout << "El usuario no existe.\n";
            return;
        }

        if (!this->relacion(pos_usuario, pos_amigo)) {
            cout << "Solo puedes visitar el perfil de un amigo.\n";
            return;
        }

        Usuario* amigo = this->get_usuario(pos_amigo);

        int total = amigo->get_cantidad_albumes();

        if (total == 0) {
            cout << "Este usuario no tiene albumes.\n";
            return;
        }

        int* indices_visibles = new int[total];
        int cantidad_visibles = 0;

        cout << "\nPerfil de: " << amigo->get_nombre() << "\n";
        cout << "\nAlbumes disponibles:\n";

        for (int i = 0; i < total; i++) {

            Album* album = amigo->get_album(i);

            if (album != nullptr &&
                album->puede_ver(visitante, true)) {

                indices_visibles[cantidad_visibles] = i;

                cout << cantidad_visibles + 1 << ". "
                     << album->get_nombre()
                     << " (" << album->get_privacidad() << ")\n";

                cantidad_visibles++;
            }
        }

        if (cantidad_visibles == 0) {
            cout << "No hay albumes disponibles.\n";
            delete[] indices_visibles;
            return;
        }

        cout << "0. Volver\n";

        string entrada;
        int opcion;

        while (true) {

            cout << "\nSelecciona un album: ";

            if (!getline(cin, entrada)) {
                delete[] indices_visibles;
                return;
            }

            if (!this->convertir_entero(entrada, opcion)) {
                cout << "Opcion invalida.\n";
                continue;
            }

            if (opcion == 0) {
                delete[] indices_visibles;
                return;
            }

            if (opcion < 1 || opcion > cantidad_visibles) {
                cout << "Opcion invalida.\n";
                continue;
            }

            break;
        }

        int indice_real = indices_visibles[opcion - 1];

        delete[] indices_visibles;

        Album* seleccionado = amigo->get_album(indice_real);

        if (seleccionado != nullptr)
            seleccionado->ver_carrusel(visitante, true);
    }


}

