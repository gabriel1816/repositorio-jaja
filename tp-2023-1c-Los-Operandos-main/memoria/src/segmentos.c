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

void borrarSegmento(t_tabla_memoria* tabla, int id) 
{
	pid_t pid;
    t_segmento* segmento_a_borrar = &tabla->tabla_segmentos[id];
    log_info(logger, "PID: %u - Eliminar Segmento: %d - Base: %u - TAMAÑO: %u", tabla->pid, id, segmento_a_borrar->base, espacio_hueco(segmento_a_borrar));
    agregar_hueco(segmento_a_borrar);
    tabla->tabla_segmentos[id].id = -1;
}

uint32_t espacio_hueco(t_segmento* segmento)
{
	return segmento->limite - segmento->base;
}

void agregar_hueco(t_segmento* segmento_a_borrar)
{
    int j = 0;
    for(int i = 0; i < list_size(lista_huecos); i++) {
        t_segmento* hueco = list_get(lista_huecos, i); 
        if(hueco->base == segmento_a_borrar->limite) {

            hueco->base = segmento_a_borrar->base;
            segmento_a_borrar->limite = hueco->limite;
            j++;
        }
        else if (hueco->limite == segmento_a_borrar->base) {
            hueco->limite = segmento_a_borrar->limite;
            segmento_a_borrar->base = hueco->base;
            j++;
        }
        if (j == 2) return;
    }
    list_add_sorted(lista_huecos, segmento_a_borrar, ordenar_segun_base);
    return;
}

bool ordenar_segun_base(t_segmento* seg_1, t_segmento* seg_2)
{
	return seg_1->base < seg_2->base;
}

void borrar_segmento(pid_t pid, int idSegmento, int una_conexion) {
    t_tabla_memoria* proceso;
     for(int i = 0; i < list_size(procesos_en_memoria); i++) {
        proceso = list_get(procesos_en_memoria, i);
        if(proceso->pid == pid) {
            break;

            }
     }
    borrar_segmento_en_memoria(proceso, idSegmento);
    enviar_proceso(proceso, una_conexion);
}

void borrar_segmento_en_memoria(t_tabla_memoria* proceso, int idSegmento) {
    t_segmento* segmentoALiberar = &proceso->tabla_segmentos[idSegmento];
    log_info(logger, "PID: %u - Eliminar Segmento: %d - Base: %u - TAMAÑO: %u", proceso->pid, idSegmento, segmentoALiberar->base, obtenerTamanioHueco(segmentoALiberar));
    agregar_hueco(segmentoALiberar);
    proceso->tabla_segmentos[idSegmento].id = -1;
    //free(segmentoALiberar);
}

int obtenerTamanioHueco(t_segmento* huecoLibre) {
    return huecoLibre->limite - huecoLibre->base;
}