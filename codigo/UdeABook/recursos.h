
#ifndef RECURSOS_H
#define RECURSOS_H

#include <string>

using namespace std;

class Recursos
{
private:
    string ruta;
    string* lineas;
    int cantidad_lineas;
    int capacidad_lineas;

public:
    Recursos();
    Recursos(string ruta);
    Recursos(const Recursos& otro);

    ~Recursos();

    const Recursos& operator=(const Recursos& otro);
    bool operator==(const Recursos& otro) const;

    void set_ruta(string ruta);
    string get_ruta() const;

    int get_cantidad_lineas() const;
    string get_linea(int posicion) const;

    bool cargar();
    bool guardar() const;

    bool agregar_linea(string texto);
    bool modificar_linea(int posicion, string texto);
    bool eliminar_linea(int posicion);
};

#endif
