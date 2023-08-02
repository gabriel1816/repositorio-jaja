#include "kernel.h"


void solicitud_memoria(pid_t pid, t_estado solicitud)
{
    t_list* parametros = list_create();
    int pid_int = (int)pid;
    char* pid_str = string_itoa(pid_int);
    list_add(parametros, pid_str);
    t_instruccion* instruccion_memoria = crear_instruccion(solicitud, parametros);
    pedir_memoria(instruccion_memoria, memoria_socket, logger);
}