#include "kernel.h"

t_tabla_memoria* solicitar_proceso_memoria(pid_t pid)
{
	t_list* parametros = list_create();
    int pid_int = (int)pid;
    char* pid_str = string_itoa(pid_int);
    list_add(parametros, pid_str);
    t_instruccion* instruccion_memoria = crear_instruccion(CREAR_PROCESO, parametros);
    pedir_memoria(instruccion_memoria, memoria_socket, logger);
    t_tabla_memoria* tabla = recibir_memoria(memoria_socket, logger);
    return tabla;
}
