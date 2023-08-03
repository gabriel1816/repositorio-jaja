#include "fileSystem.h"


void atender_kernel(void* socket_kernel)
{
	
	t_pcb* pcb = recibir_pcb((int)socket_kernel, logger);/*
	t_instru
	switch(instruccion->identificador){
		t_buffer *buffer = malloc(sizeof(t_buffer));
		t_paquete *paquete;
		char *mensaje;
		case F_OPEN: 
			bool abrioElArchivo = abrir_archivo(instruccion->parametros[0]);
			if(abrioElArchivo){
				mensaje = "se pudo abrir el archivo";
			}
			else{
				mensaje = "No se pudo abrir el archivo, el archivo no existe";
			}
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));
			enviar_paquete(socket_kernel, paquete, logger);

		case F_READ: 
			t_direc_fisica *direcFisica_lectura = traducir_direccion(instruccion->parametros[1]);
			char* info_a_leer = malloc(instruccion->parametros[2] + 1);
			
			leer_archivo(instruccion->parametros[0], info_a_leer ,instruccion->parametros[2]);
			
			//le mando a memoria lo que se leyo
			mandar_a_memoria(info_a_leer, direcFisica_lectura, conexionConMemoria, instruccion);

			//le mando a kernel que se leyo el archivo
			mensaje = "se leyo el archivo";
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));
			enviar_paquete(socket_kernel, paquete, logger);

		case F_WRITE: 
		//le solicita a memoria la info que se encuentra a partir de la direc fisica
			char datos_para_escribir = pedir_a_memoria(instruccion->parametros[1], conexionConMemoria);
			
			escribir_archivo(instruccion->parametros[0],datos_para_escribir, instruccion->parametros[2]);
			
			//le mando a kernel que se escribio el archivo
			mensaje = "se escribio el archivo";
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));
			enviar_paquete(socket_kernel, paquete, logger);

		case F_CLOSE:
		t_fcb *fcb_eliminar = obtenerFcb(instruccion->parametros[0]);
		list_remove(lista_fcbs, fcb_eliminar);//elimino de la lista de fcbs el fcb de ese arch 

		// eimino la entrada del directorio

		case F_TRUNCATE:
			//truncar_archivo(instruccion->parametros[0]);
			mensaje = "se modifico el tamanio del archivo";
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));
			enviar_paquete(socket_kernel, paquete, logger);
		case F_SEEK:
	}
}

char pedir_a_memoria(t_instruccion* instruccion, int conexionConMemoria){
	t_buffer *buffer = malloc(sizeof(t_buffer));
	buffer = crear_buffer_para_t_instruccion(instruccion);
	t_paquete* paquete = crear_paquete(buffer, instruccion->identificador);
	enviar_paquete(conexionConMemoria, paquete, logger);

	// recibimos valor de memoria
	recv(conexionConMemoria, &(paquete->codigo_operacion), sizeof(uint8_t), 0);

	// deserializamos paquete
	char valor;
	void* stream = paquete->buffer->stream;
	memcpy(&(valor), stream, sizeof(char));

	return valor;
	*/
destruir_pcb(pcb);
}

void mandar_a_memoria(char* valor, t_direc_fisica* direccion, int socketMemoria, t_instruccion *instruccion){

	t_buffer *buffer = malloc(sizeof(t_buffer));
	t_paquete* paquete = crear_paquete(buffer, instruccion->identificador);
	agregar_a_paquete(paquete, (void*)direccion, sizeof(t_direc_fisica));
	agregar_a_paquete(paquete, valor, strlen(valor));
	enviar_paquete(socketMemoria, paquete, logger);
	eliminar_paquete(paquete);

}