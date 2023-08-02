#include "CPU.h"


//RECORDAR CAMBIAR TODOOOOOOOOOOOOOOOOOOOOOOOOOOOOO

char* MOV_IN_memoria(pid_t pid, uint32_t direccion_fisica, uint32_t registro) { 
    t_list* parametros = list_create();
	char* pidString = string_itoa((int)pid);
    char* direccion_fisica_string = string_itoa(direccion_fisica);
	list_add(parametros, pidString);
	list_add(parametros, direccion_fisica_string);
	list_add(parametros, registro);
    t_instruccion* pedidoMemoria = crear_instruccion(MOV_IN, parametros);
    pedir_memoria(pedidoMemoria, conexion_con_memoria, logger);

	t_paquete *paquete = recibir_paquete(conexion_con_memoria, logger);
	int offset=0;
    t_instruccion* respuestaMemoria = crear_instruccion_para_el_buffer(paquete->buffer,&offset);
	return respuestaMemoria->parametros[0];
}

void MOV_OUT_memoria(pid_t pid, uint32_t direccion_fisica, char* valor_leido) {

	t_list* parametros = list_create();
	char* pidString = string_itoa(pid);
    char* direccion_fisica_string = string_itoa(direccion_fisica);
	list_add(parametros, pidString);
	list_add(parametros, direccion_fisica_string);
	list_add(parametros, valor_leido);
    t_instruccion* pedidoMemoria = crear_instruccion(MOV_OUT, parametros);
    pedir_memoria(pedidoMemoria, conexion_con_memoria, logger);

	t_paquete *paquete = recibir_paquete(conexion_con_memoria, logger);
    if(paquete->codigo_operacion == MOV_OUT) log_info(logger, "El MOV_OUT resulto exitoso");
    destruir_paquete(paquete);
	return;
}

