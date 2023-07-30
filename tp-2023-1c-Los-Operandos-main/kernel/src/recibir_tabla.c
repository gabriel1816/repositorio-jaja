#include "kernel.h"
/*

t_list* deserializar_tabla_segmentos(void* stream, int* desplazamiento)
{
	t_list* tabla_segmentos= list_create();

	int cant_segmentos;
    cant_segmentos = stream + *desplazamiento;
	desplazamiento+= sizeof(int);

	int tamanio;

	for(int j=0; j<cant_segmentos; j++){
		t_segmento segmento;
		memcpy(&tamanio, stream + *desplazamiento, sizeof(int));
		*desplazamiento += sizeof(int);
		memcpy(&segmento, stream + *desplazamiento, sizeof(t_segmento));
		*desplazamiento += sizeof(t_segmento);
		list_add(tabla_segmentos, &segmento);
	}
	return tabla_segmentos;
}


t_tabla_segmentos* deserializar_segmentos(void* stream, int* desplazamiento){
	t_tabla_segmentos* tabla = malloc(sizeof(t_tabla_segmentos));
	tabla->segmentos = list_create();
	int pid = deserializar_uint32(stream, desplazamiento);
	tabla->pid = pid;
	int cant_segmentos = deserializar_uint32(stream, desplazamiento);
	for(int j=0; j < cant_segmentos; j++){
		t_segmento* segmento = malloc(sizeof(t_segmento));
		memcpy(&(segmento->base), stream + *desplazamiento, sizeof(void*));
		*desplazamiento += sizeof(void*);
		memcpy(&(segmento->limite), stream + *desplazamiento, sizeof(void*));
		*desplazamiento += sizeof(void*);
		segmento->pid = pid;
		segmento->id = j;
		if(segmento->base != NULL){
			segmento->libre = 0;
		} else{
			segmento->libre = 1;
		}

		list_add(tabla->segmentos, segmento);

	}
	return tabla;
}
*/