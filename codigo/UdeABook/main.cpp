#include "recursos.h"
#include <iostream>
#include <string>
#include "redsocial.h"
#include "recursos.h"
#include "interfaz.h"
using namespace std;
int main(int argc, char **argv)
{
    RegistroLocal memoria_1(static_cast<unsigned int>(sizeof(argc)) + static_cast<unsigned int>(sizeof(argv)));

    try
    {
        string carpeta = argc > 1 ? argv[1] : ".";
        RegistroLocal memoria_2(static_cast<unsigned int>(sizeof(carpeta)));

        RedSocial red;
        RegistroLocal memoria_3(static_cast<unsigned int>(sizeof(red)));

        {
            Recursos recursos("Carga inicial");
            if (!red.cargar_usuarios(carpeta + "/usuarios.txt") ||
                !red.cargar_albumes(carpeta + "/albumes.txt") ||
                !red.cargar_mensajes(carpeta + "/mensajes.txt"))
            {
                recursos.mostrar();
                {
                    contar_llamadas(9, 1);
                    cout << "No se modificaron los archivos. Revise el conjunto de datos.\n";
                }
                return 1;
            }
            recursos.mostrar();
        }
        unsigned int usuario = red.get_cantidad_usuarios();
        RegistroLocal memoria_4(static_cast<unsigned int>(sizeof(usuario)));

        unsigned int perfiles[3];
        RegistroLocal memoria_5(static_cast<unsigned int>(sizeof(perfiles)));

        unsigned int cantidad_perfiles = 0;
        RegistroLocal memoria_6(static_cast<unsigned int>(sizeof(cantidad_perfiles)));

        bool actualizados = false;
        RegistroLocal memoria_7(static_cast<unsigned int>(sizeof(actualizados)));

        {
            Recursos recursos("Ingreso y fotos de perfil");
            string nombre, contra;
            RegistroLocal memoria_8(static_cast<unsigned int>(sizeof(nombre)) + static_cast<unsigned int>(sizeof(contra)));

            while (true)
            {
                contar_iteracion();
                {
                    contar_llamadas(9, 1);
                    cout << "Usuario: ";
                }
                if (!(contar_llamada(0), getline(cin, nombre)))
                    return 0;
                {
                    contar_llamadas(9, 1);
                    cout << "Contrasena: ";
                }
                if (!(contar_llamada(0), getline(cin, contra)))
                    return 0;
                usuario = red.buscar_usuario(nombre);
                if (usuario < red.get_cantidad_usuarios() &&
                    red.get_usuario(usuario)->verificar_contra(contra))
                    break;
                {
                    contar_llamadas(9, 1);
                    cout << "Usuario o contrasena incorrectos.\n";
                }
            }
            red.mostrar_perfiles(usuario, perfiles, cantidad_perfiles, false);
            recursos.mostrar();
        }
        while (true)
        {
            contar_iteracion();
            unsigned int opcion;
            RegistroLocal memoria_9(static_cast<unsigned int>(sizeof(opcion)));

            if (!pedir_numero("1. Solicitudes\n2. Visitar amigo\n3. Sugerencias\n4. Enviar mensaje\n5. Leer "
                              "no leidos\n6. Mensajes por fechas\n7. Actualizar perfiles (una vez)\n0. "
                              "Guardar y salir\nOpcion: ",
                              0, 7, opcion))
                opcion = 0;
            if (opcion == 0)
            {
                Recursos recursos("Guardar datos y cerrar sesion");
                bool correcto = red.guardar_usuarios();
                RegistroLocal memoria_10(static_cast<unsigned int>(sizeof(correcto)));

                if (correcto)
                    correcto = red.guardar_albumes(carpeta + "/albumes.txt");
                if (correcto)
                    correcto = red.guardar_mensajes(carpeta + "/mensajes.txt");
                recursos.mostrar();
                if (!correcto)
                {
                    {
                        contar_llamadas(9, 1);
                        cout << "Fallo el guardado. Se conservan los datos en RAM para reintentar.\n";
                    }
                    if (!cin)
                        return 1;
                    continue;
                }
                {
                    contar_llamadas(9, 1);
                    cout << "Sesion finalizada.\n";
                }
                break;
            }
            const char *nombres[7] = {"Solicitudes",        "Visitar amigo",  "Sugerencias",
                                      "Enviar mensaje",     "Leer no leidos", "Buscar mensajes por fechas",
                                      "Actualizar perfiles"};
            RegistroLocal memoria_11(static_cast<unsigned int>(sizeof(nombres)));

            Recursos recursos(nombres[opcion - 1]);
            switch (opcion)
            {
            case 1:
                red.menu_solicitudes(usuario);
                break;
            case 2:
                red.menu_visitar_perfil_amigo(usuario);
                break;
            case 3:
                red.menu_sugerencias(usuario);
                break;
            case 4:
                red.menu_enviar_mensaje(usuario);
                break;
            case 5:
                red.leer_mensajes_nuevos(usuario);
                break;
            case 6:
                red.menu_buscar_mensajes_por_fecha(usuario);
                break;
            case 7:
                if (actualizados)
                {
                    contar_llamadas(9, 1);
                    cout << "Ya actualizaste los perfiles en esta sesion.\n";
                }
                else
                {
                    red.mostrar_perfiles(usuario, perfiles, cantidad_perfiles, true);
                    actualizados = true;
                }
                break;
            }
            recursos.mostrar();
            if (red.get_error_fatal())
            {
                {
                    contar_llamadas(9, 1);
                    cout << "La operacion fallo. Terminando sin sobrescribir archivos.\n";
                }
                return 1;
            }
        }
        return 0;
    }
    catch (...)
    {
        {
            contar_llamadas(9, 1);
            cout << "No se pudo completar la operacion por un error de memoria o de ejecucion.\n";
        }
        return 1;
    }
}
