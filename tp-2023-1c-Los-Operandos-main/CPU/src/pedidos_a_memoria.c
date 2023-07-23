#include "CPU.h"


char pedir_a_memoria(t_direc_fisica* direccion, int conexionConMemoria){

	// mandamos direccion a memoria
	t_paquete* paquete = crear_paquete();
	agregar_a_paquete(paquete, (void*)direccion, sizeof(t_direc_fisica));
	enviar_paquete(paquete, conexionConMemoria);
	eliminar_paquete(paquete);

	// recibimos valor de memoria
	recv(conexionConMemoria, &(paquete->codigo_operacion), sizeof(uint8_t), 0);

	// deserializamos paquete
	char valor;
	void* stream = paquete->buffer->stream;
	memcpy(&(valor), stream, sizeof(char));

	return valor;
}

void mandar_a_memoria(char* valor, t_direc_fisica* direccion, int socketMemoria){

	t_paquete* paquete = crear_paquete();
	agregar_a_paquete(paquete, (void*)direccion, sizeof(t_direc_fisica));
	agregar_a_paquete(paquete, valor, strlen(valor));
	enviar_paquete(paquete, socketMemoria);
	eliminar_paquete(paquete);

}
