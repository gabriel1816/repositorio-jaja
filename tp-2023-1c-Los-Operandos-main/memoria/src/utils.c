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
void inicializarMemoria() 
{
    memoria_fisica = calloc(tamanio_memoria, sizeof(uint32_t)); 
    //procesosEnMemoria = list_create();
    lista_huecos = list_create();
    segmento_cero.id = 0;
    segmento_cero.base = 0;
    segmento_cero.limite = tamanio_segmento_0;
    inicializar_lista_huecos(tablas_segmentos);
    segmento_vacio.id = -1;
    segmento_vacio.base = 0;
    segmento_vacio.limite = 0;

}

//void crear_estructuras()
//{
//    memoria_libre = atoi(tamanio_memoria);
//    tablas_segmentos = list_create();
//    lista_huecos = list_create();
//    algoritmo = asignar_algoritmo();
//
//    
//    //creo el bit array que me dice que segmento esta ocupado y cual no(no se si va aca esto)
//    char* bloque_memoria = (char *) atoi(tamanio_memoria);
//    t_bitarray *bitmap_memoria = bitarray_create(bloque_memoria, atoi(tamanio_memoria)); //supongo que cuando lo creo empieza con todos ceros
//   
//}


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

void inicializar_lista_huecos(t_list* tablas_segmentos){
    for(int i = 0; i < list_size(tablas_segmentos); i++){
        // agrego segmentos libres
        t_segmento *segmento = list_get(tablas_segmentos, i);

        if(segmento){ /////////!!!!!!!!!!!!!!!!!!
            list_add(lista_huecos, segmento);  
        }

    }
}
