#include "memoria.h"


void crear_proceso_memoria(pid_t pid, int conexion_cpu) 
{
    t_tabla_memoria* nuevoProceso = malloc(sizeof(t_tabla_memoria));
    t_segmento* tabla_segmento = calloc((uint32_t)cantidad_segmentos * sizeof(t_segmento), sizeof(int));
    tabla_segmento[0] = segmento_cero;
    nuevoProceso->pid = pid;
    nuevoProceso->tabla_segmentos = tabla_segmento;
    nuevoProceso->tam_tabla = cantidad_segmentos;
    list_add(procesos_en_memoria, nuevoProceso); 
    for(int i = 1; i < cantidad_segmentos; i++) {
        tabla_segmento[i] = segmento_vacio; 
    }
    log_info(logger, "Creacion de Proceso PID: %u", pid);
    enviar_proceso(nuevoProceso, conexion_cpu);
}

void enviar_proceso(t_tabla_memoria* nuevoProceso, int socket)
{
    t_buffer* buffer = crear_buffer_tabla_memoria(nuevoProceso);
    t_paquete* paquete = crear_paquete(buffer, CODIGO_INSTRUCCION_MEMORIA);
    enviar_paquete(socket, paquete, logger);
	destruir_paquete(paquete);
}

void eliminar_proceso(pid_t pid)
{
    for(int i=0; i < list_size(procesos_en_memoria); i++){
        t_tabla_memoria* tabla = list_get(procesos_en_memoria, i);
        if(tabla->pid == pid){
            eliminar_segmentos(tabla);
            list_remove(procesos_en_memoria, i);
        }
    free(tabla->tabla_segmentos);
    free(tabla);
    }
    log_info(logger, "Eliminacion de Proceso PID: %u", pid);
}


void eliminar_segmentos(t_tabla_memoria* tabla)
{
    for(int i=1; i < tabla->tam_tabla; i++){
        int id = tabla->tabla_segmentos[i].id;
        if(id != -1){
            borrarSegmento(tabla, id);
        }
    }
}

