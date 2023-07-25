#include "memoria.h"


t_paquete* serializar_segmentos(t_tabla_segmentos* tabla){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer = NULL;
	t_paquete* paquete = crear_paquete(buffer, 7);
	agregar_a_paquete(paquete, &(tabla->pid), sizeof(int));
	int cant_segmentos = list_size(tabla->segmentos);
	agregar_a_paquete(paquete, &cant_segmentos, sizeof(int));
	for(int i = 0; i < list_size(tabla->segmentos); i++){
		t_segmento* segmento = list_get(tabla->segmentos, i);
		agregar_a_paquete(paquete, &(segmento->base), sizeof(void*));
		agregar_a_paquete(paquete, &(segmento->limite), sizeof(void*));
	}
	return paquete;
}

void enviar_segmentos(t_tabla_segmentos* tabla, int socket){
	t_paquete* paquete;
	paquete = serializar_segmentos(tabla);
	int tamanio_tabla;
	memcpy(&tamanio_tabla, paquete->buffer->stream, sizeof(int));
	enviar_paquete( socket, paquete, logger);
}

