#ifndef CPU_H_
#define CPU_H_


#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "utils-shared.h"
#define CONFIG_PATH "./cfg/CPU.config"
#define LOG_PATH "./cfg/CPU.log"


//************* VARIABLES  *************//
//************* GLOBALES   *************//

extern t_registro registro_cpu;

extern t_log* logger;

extern int conexionConMemoria;
extern char *ipCPU;
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

//************* REGISTROS *************//

void guardarRegistro(char* registro, char* parametro, t_registro* registros);
char* leerRegistro(char* registro, t_registro* registros);
void mostrar_por_pantalla(t_registro registro);

//************** MEMORIA *************//

//char pedir_a_memoria(direccionFisica* direccion, int conexionConMemoria);
//void mandar_a_memoria(char* valor, direccionFisica* direccion, int socketMemoria);

//*************** MMU ***************//

int32_t traducir_direccion(uint32_t direccion_logica, t_pcb *pcb, int conexion_kernel, uint32_t cantidad_bytes);
uint32_t obtener_numero_segmento(uint32_t direccion_logica);
uint32_t obtener_desplazamineto_segmento(uint32_t direccion_logica);

//************** CICLO *************//
void comenzar_ciclo_de_instruccion(t_pcb* pcb, int conexion_con_kernel);
t_instruccion* fetch(t_pcb * pcb);
void decode_y_execute(t_instruccion* instruccion_a_ejecutar, t_pcb* pcb, int conexion_con_kernel);

#endif /* CPU_H_ */