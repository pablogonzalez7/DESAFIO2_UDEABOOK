#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <string>
#include <fstream>
using namespace std;
unsigned int longitud_texto(const string &texto);
string parte_texto(const string &texto, unsigned int inicio, unsigned int cantidad);
bool leer_linea(ifstream &archivo, string &linea);
bool tomar_campo(const string &linea, unsigned int &posicion, string &campo, bool ultimo);
bool numero_natural(const string &texto, unsigned int &numero);
string texto_numero(unsigned int numero);
bool campo_simple(const string &texto);
class Archivo
{
private:
    string ruta;
    string *lineas;
    unsigned int cantidad_lineas;
    unsigned int capacidad_lineas;

public:
    Archivo();
    Archivo(const string &ruta);
    Archivo(const Archivo &otro);
    ~Archivo();
    const Archivo &operator=(const Archivo &otro);
    bool operator==(const Archivo &otro) const;
    void set_ruta(const string &ruta);
    string get_ruta() const;
    unsigned int get_cantidad_lineas() const;
    const string &get_linea(unsigned int posicion) const;
    bool cargar();
    bool guardar() const;
    bool reservar(unsigned int capacidad);
    bool agregar_linea(const string &texto);
    bool modificar_linea(unsigned int posicion, const string &texto);
    bool eliminar_linea(unsigned int posicion);
};
#endif
