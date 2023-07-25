#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_


#include <stdbool.h>
#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "utils-shared.h"
#define MAX_LINE_LENGTH 24

typedef struct{
	char* nombre_archivo; // preguntar
	int tam_archivo;
	uint32_t puntero_directo;
	uint32_t puntero_indirecto;
}t_fcb;

typedef struct{
	t_fcb* fcb;
	t_fcb* siguiente_fcb;
}t_lista_fcb;

typedef enum {
		LSB_FIRST,
		MSB_FIRST
	} bit_numbering_t;

typedef struct {
		char *bitarray;
		size_t size;
		bit_numbering_t mode;
	} t_bitarray;

extern t_log* logger;
extern t_config *config;
extern int conexionConMemoria;
extern char *ipMemoria;
extern char *puertoMemoria;
extern char *valorMemoria;
extern char *puertoEscucha;
extern char *path_superbloque;
extern char *path_bitmap;
extern char *path_bloques;
extern char *path_fcb;
extern char *retardo_acceso_bloque;
extern int tamanio_bloque;
extern int cant_bloques; 
extern int tam_max_segmento;
extern t_lista_fcb* lista_fcbs[25];



t_log* iniciar_logger(void);
t_config* iniciar_config(void);
void terminar_programa(int, t_log*, t_config*);
char pedir_a_memoria(t_direc_fisica* direccion, int conexionConMemoria, t_instruccion *instruccion);
void mandar_a_memoria(char* valor, t_direc_fisica* direccion, int socketMemoria, t_instruccion *instruccion);
t_direc_fisica* traducir_direccion(int direc_logica);
void atender_instruccion(t_instruccion *instruccion, int socket_kernel);
int cant_bloques_nec(int tam_archivo);
void terminar_programa(int conexion, t_log *logger, t_config *config) ;
t_config* iniciar_config(void);
t_log* iniciar_logger(void);
void levantar_estructuras();
t_lista_fcb* agregar_a_Lista(t_lista_fcb* lista, t_fcb* fcb_a_agregar);
bool abrir_archivo(char* nombre_archivo);
bool crear_archivo(char *nombreArchivo);
void leer_archivo(char *path_archivo, void* info_a_leer, int cantBytes);
void escribir_archivo(char *path_archivo, char info_a_escribir, int cantBytes);
t_fcb* obtenerFcb(char *nombre_archivo);
int bloqueLogicoAFisico(t_fcb* fcb, int num_bloque);
void borrar_bloques(int bloques_ocupados, int cant_bloques_a_borrar, t_fcb *fcb);
void agregar_bloques(int bloques_ocupados, int cant_bloques_a_agregar, t_fcb *fcb);
uint32_t asignar_bloque_libre();


#endif /* FILESYSTEM_H_ */