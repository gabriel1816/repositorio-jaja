#include "memoria.h"

t_segmento* crear_segmento(void* base, void* limite, int id, bool libre, pid_t pid)
{
	t_segmento* segmento = malloc(sizeof(t_segmento));
	segmento->base = base;
	segmento->limite = limite;
	segmento->id = id;
	if (libre == 0){
		memoria_libre -= (limite - base);
	}
	//ocupar_bitmap((base - memoria_fisica), (limite - base));
	//printf("\nMemoria Libre %d \n", memoria_libre);
	return segmento;
}

void borrarSegmento(t_segmento* segmento)
{
	memoria_libre += (segmento->limite - segmento->base);
	t_segmento* hueco_nuevo = malloc(sizeof(t_segmento));
	hueco_nuevo->base = segmento->base;
	hueco_nuevo->limite = segmento->limite;
	hueco_nuevo->id = 0;
	segmento->base = NULL;
	segmento->limite = NULL;
	//insertar_ordenado(hueco_nuevo, lista_huecos, 1);
}

t_tabla_segmentos* crearTabla(pid_t pid)
{
	t_tabla_segmentos* tabla = malloc(sizeof(t_tabla_segmentos));
	tabla->segmentos = list_create();
	list_add(tablas_segmentos, tabla);
	list_add_in_index(tabla->segmentos,0, &segmento_cero);
	log_info(logger, "Creación de Proceso PID: %d", pid);
	for(int i = 1; i < atoi(cantidad_segmentos); i++){
		t_segmento* segmento = malloc(sizeof(t_segmento));
		segmento = crear_segmento(NULL, NULL, i, 1, pid);
		list_add_in_index(tabla->segmentos,i, segmento);
	}
	return tabla;
}

void borrarTabla(t_tabla_segmentos* tabla)
{
	for(int i = 1; i < atoi(cantidad_segmentos); i++){
		t_segmento* segmento = list_get(tabla->segmentos,i);
		//fin_segmento(segmento, i);
	}
}