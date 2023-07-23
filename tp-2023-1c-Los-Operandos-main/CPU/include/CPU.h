#ifndef CPU_H_
#define CPU_H_


#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "utils-shared.h"


extern t_registro registro_cpu;

extern t_log* logger;

extern int conexionConMemoria;
extern char *ipMemoria;
extern char *puertoMemoria;
extern char *valorMemoria;
extern char *puerto_escucha;
extern int tam_max_segmento;
extern int retardo_instruccion;
extern t_temporal* temporizador;


t_log* iniciar_logger(void);
t_config* iniciar_config(void);
void terminar_programa(int, t_log*, t_config*);
void atender_kernel(void* conexion);
void guardarRegistro(char* registro, char* parametro, t_registro* registros);
char* leerRegistro(char* registro, t_registro* registros);

//char pedir_a_memoria(direccionFisica* direccion, int conexionConMemoria);
//void mandar_a_memoria(char* valor, direccionFisica* direccion, int socketMemoria);

t_pcb* recibir_pcb(int socket_cliente);
t_pcb* deserializar_buffer_pcb(t_buffer* buffer);
void destruir_pcb(t_pcb* pcb);

void comenzar_ciclo_de_instruccion(t_pcb* pcb, int conexion_con_kernel);
t_instruccion* fetch(t_pcb * pcb);
void decode_y_execute(t_instruccion* instruccion_a_ejecutar, t_pcb* pcb, int conexion_con_kernel);

#endif /* CPU_H_ */