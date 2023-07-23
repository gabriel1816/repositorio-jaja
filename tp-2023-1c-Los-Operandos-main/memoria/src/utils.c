#include "memoria.h"

void levantar_config(char* path)
{

    t_config* config = config_create(path);
	puerto = config_get_string_value(config, "PUERTO_ESCUCHA");
    tamanio_memoria = config_get_int_value(config, "TAM_MEMORIA");
    tamanio_segmento_0 = config_get_int_value(config, "TAM_SEGMENTO_0");
    cantidad_segmentos = config_get_int_value(config, "CANT_SEGMENTOS");
    retardo_memoria = config_get_int_value(config, "RETARDO_MEMORIA");
    retardo_compactacion = config_get_int_value(config, "RETARDO_COMPACTACION");
    algoritmo_asignacion = config_get_string_value(config, "ALGORITMO_ASIGNACION");
    ip_memoria = config_get_string_value(config, "IP_MEMORIA");
}

void crear_estructuras()
{

    memoria_fisica = asignar_espacio_en_memoria();
    memoria_libre = atoi(tamanio_memoria);
    tablas_segmentos = list_create();
    lista_huecos = list_create();
    algoritmo = asignar_algoritmo();

    //
    // segmento_cero = crear_segmento(base, base + tamanio_segmento_0, 0,0,0);
    //tablas_segmentos = crear_tabla();
}


int asignar_espacio_en_memoria()
{
    return atoi(malloc(tamanio_memoria));
}

t_algoritmo asignar_algoritmo()
{
    if(string_equals_ignore_case(algoritmo_asignacion, "FIRST")){
        return FIRST_FIT;
    }
    if(string_equals_ignore_case(algoritmo_asignacion, "WORST")){
        return WORST_FIT;
    }
    if(string_equals_ignore_case(algoritmo_asignacion, "BEST")){
        return BEST_FIT;
    }
    log_error(logger, "Algoritmo invalido");
    return;
}