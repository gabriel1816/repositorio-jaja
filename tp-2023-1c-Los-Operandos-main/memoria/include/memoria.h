#ifndef MEMORIA_H_
#define MEMORIA_H_


#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h> 
#include "utils-shared.h"


typedef struct{
    pid_t pid;
    //int tamanio;
} t_new_procesos;

typedef enum{
    BEST_FIT,
    WORST_FIT,
    FIRST_FIT
}t_algoritmo;


extern int memoria_libre;
extern t_list* tablas_segmentos;
extern t_segmento* segmento_cero;
extern int memoria_libre;
extern void* memoria_fisica;
extern t_list* lista_huecos;
extern t_algoritmo algoritmo;

extern char*  puerto;
extern int* tamanio_memoria;
extern int* tamanio_segmento_0;
extern int* cantidad_segmentos;
extern int* retardo_memoria;
extern int* retardo_compactacion;
extern char* algoritmo_asignacion;
extern char* ip_memoria;


void atender_cpu(void* conexion, t_log* logger);
void atender_kernel(void* conexion, t_log* logger);
void atender_fs(void* conexion, t_log* logger);

void levantar_config(char* path);
void crear_estructuras();
int asignar_espacio_en_memoria();
t_algoritmo asignar_algoritmo();
t_segmento* crear_segmento(void* base, void* limite, int id, bool libre, pid_t pid);
void borrarSegmento(t_segmento* segmento);
t_tabla_segmentos* crearTabla(pid_t pid);
void borrarTabla(t_tabla_segmentos* tabla);
t_tabla_segmentos* crearTabla(pid_t pid);


extern pthread_t hilo_de_escucha_cpu;
extern pthread_t hilo_de_escucha_fs;
extern pthread_t hilo_de_escucha_kernel; 

#endif /* MEMORIA_H_ */