#ifndef MEMORIA_H_
#define MEMORIA_H_


#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h> 
#include "utils-shared.h"
#define LOG_PATH "../cfg/memoria.log"
#define CONFIG_PATH "../cfg/memoria.config"

//************ ESTRUCTURAS   ************//

typedef struct{
    pid_t pid;
    //int tamanio;
} t_new_procesos;

typedef enum{
    BEST_FIT,
    WORST_FIT,
    FIRST_FIT
}t_algoritmo;

typedef enum {
	LSB_FIRST,
	MSB_FIRST
} bit_numbering_t;

typedef struct {
	char *bitarray;
	size_t size;
	bit_numbering_t mode;
} t_bitarray;

//************* VARIABLES  *************//
//************* GLOBALES   *************//

extern int memoria_libre;
extern t_list* tablas_segmentos;
extern int memoria_libre;
extern void* memoria_fisica;
extern t_list* lista_huecos;
extern t_algoritmo algoritmo;

extern char*  puerto;
extern int tamanio_memoria;
extern int tamanio_segmento_0;
extern int cantidad_segmentos;
extern int retardo_memoria;
extern int retardo_compactacion;
extern char* algoritmo_asignacion;
extern char* ip_memoria;
extern t_log* logger;
extern pthread_t hilo_de_escucha_cpu;
extern pthread_t hilo_de_escucha_fs;
extern pthread_t hilo_de_escucha_kernel; 
extern t_list* procesos_en_memoria;
extern t_segmento segmento_cero;
extern t_segmento segmento_vacio;

//************* -----------  *************//

void levantar_config(char* path);
void crear_estructuras();
int asignar_espacio_en_memoria();
void inicializarMemoria();

//************* CONEXIONES  *************//

void escuchar(int puerto_escucha);
void atender_cpu(void* conexion);
void atender_kernel(void* conexion);
void atender_fs(void* conexion);
void escucha_general(void* conexion);
void escucha_general(void* conexion);


//************* SEGMENTOS  *************//

t_algoritmo asignar_algoritmo();
t_segmento* crear_segmento(void* base, void* limite, int id, bool libre, pid_t pid);
void eliminar_segmentos(t_tabla_memoria* tabla);
void borrarSegmento(t_tabla_memoria* tabla, int id);
bool hay_espacio(t_list* lista_huecos);
bool hay_huecos_continuos(int cant_huecos_nec, t_algoritmo algoritmo, int tamanio_seg, t_list* huecos_libres);
t_segmento* first_fit();
t_segmento* worst_fit(int tamanio_necesitado);
t_segmento* best_fit(int tamanio_necesitado);
void compactar(t_list* tabla_segmentos);
bool segmento_es_hueco(t_segmento *segmento, t_list* lista_huecos);
void modificar_valores_segmento(t_segmento *segmento, t_list* tabla_seg);
void unir_todos_los_huecos(t_list *tabla_segmentos);
t_segmento* primer_hueco(t_list *tabla_segmentos);
uint32_t espacio_hueco(t_segmento* segmento);
void eliminar_segmentos(t_tabla_memoria* tabla);
bool ordenar_segun_base(t_segmento* seg_1, t_segmento* seg_2);
void agregar_hueco(t_segmento* segmento_a_borrar);

//************* PROCESOS  *************//

void crear_proceso_memoria(pid_t pid, int conexion_cpu);
void enviar_proceso(t_tabla_memoria* nuevoProceso, int socket);
bool hay_espacio(t_list *segmentos_vacios);
int obtener_tam_segmento(t_segmento *segmento);
bool hay_huecos_continuos(int cant_huecos_libres, t_algoritmo algoritmo_asignacion, int tamanio, t_list *huecos_disponibles);
t_segmento* first_fit();
t_segmento* best_fit(int tamanio_necesitado);
t_segmento* worst_fit(int tamanio_necesitado);
void eliminar_proceso(pid_t pid);
void borrar_segmento(pid_t pid, int idSegmento, int una_conexion) ;
int obtenerTamanioHueco(t_segmento* huecoLibre);

#endif /* MEMORIA_H_ */