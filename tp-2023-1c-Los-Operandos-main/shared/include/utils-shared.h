#ifndef SHARED_UTILS_H
#define SHARED_UTILS_H


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <commons/log.h>
#include <commons/temporal.h>
#include <commons/collections/list.h>
#include<commons/config.h>
#include <assert.h>
#include <commons/string.h>
#include <stdint.h>
#include <commons/collections/dictionary.h>

#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <commons/collections/queue.h>
#include <semaphore.h>

#define INITIAL_STREAM_SIZE 64
#define CODIGO_INSTRUCCION 20
#define CODIGO_PCB 80

extern t_log* logger;

typedef enum
{
	MENSAJE,
	PAQUETE,
	INSTRUCCION,
	PCB
}op_code;

typedef struct {
	int size;
	void* stream;
}t_buffer;

typedef struct
{
	uint32_t codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct {
    t_list* segmentos;
	int pid;
} t_tabla_segmentos;
//^^^^mmmmmmm


typedef struct{
    int id;
    // pid_t pid;
    void* base;
    void* limite;
    // int tamanio;
    // bool libre;
} t_segmento;


typedef enum {
	SET,
	MOV_OUT,
	WAIT,
	I_O,
	SIGNAL,
	MOV_IN,
	F_OPEN,
	YIELD,
	F_TRUNCATE,
	F_SEEK,
	CREATE_SEGMENT,
	F_WRITE,
	F_READ,
	DELETE_SEGMENT,
	F_CLOSE,
	EXIT,
	CONTEXTO 
}t_identificador;

typedef struct{
	t_identificador identificador;
	uint32_t cant_parametros;
	uint32_t param_length1;
	uint32_t param_length2;
	uint32_t param_length3;
	uint32_t param_length4;
	char **parametros;

}t_instruccion;

typedef struct{
    char AX[5], BX[5], CX[5], DX[5];
    char EAX[9], EBX[9], ECX[9], EDX[9];
    char RAX[17], RBX[17], RCX[17], RDX[17];
}t_registro;

typedef enum{
	NUEVO,
	LISTO,
	EJECUTANDO,
	BLOQUEADO,
	TERMINADO
}t_estado;

typedef struct{
	int conexion; //para cerrar el proceso 
    pid_t pid; // Identificador del proceso (averiguar como hacer para que sea unico en el sistema)
	int p_counter;
    t_estado estado; // Estado del proceso (ejecución, listo, bloqueado, etc.)
	t_list* instrucciones; // Lista de instrucciones
    t_temporal* tiempo_llegada;		
    int64_t tiempo_ejecucion;
	t_registro registros;	// Contexto de la cpu
	double estimado_rafaga; 
	t_segmento* tabla_segmentos;
	uint32_t tamanio_tabla;
	//archivos
	uint32_t direccion_fisica;
}t_pcb;


typedef struct
{
	int numero_segmento;
	int desplazamiento;
}t_direc_fisica;



int crear_conexion(char* ip, char* puerto);
t_paquete* crear_super_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void* recibir_buffer(int* size, int socket_cliente);
void destruir_buffer(t_buffer* buffer);
uint32_t espacio_de_array_parametros(t_instruccion *instruccion);
t_paquete *crear_paquete(t_buffer *buffer, int codigo_operacion);
bool enviar_instrucciones(int socket, t_list *lista_instrucciones, t_log *logger);
t_buffer *crear_buffer_para_t_instruccion(t_instruccion *instruccion);
bool enviar_paquete(int socket, t_paquete *paquete, t_log *logger);
void destruir_paquete(t_paquete *paquete);
t_paquete *recibir_paquete(int socket, t_log *logger);
t_buffer* crear_buffer();

t_buffer *null_buffer();
t_list *crear_lista_instrucciones_para_el_buffer( t_buffer *buffer);
t_instruccion *crear_instruccion_para_el_buffer(t_buffer *buffer, uint32_t *offset);
t_buffer *crear_buffer__para_t_lista_instrucciones(t_list *lista_instrucciones);
void enviar_pcb(t_pcb* pcb, int socket, t_log* logger);
t_pcb* crear_pcb(int pid, t_list* lista_instrucciones, t_estado estado, double estimado_rafaga);
t_registro inicializar_registro();
t_buffer* crear_buffer_pcb(t_pcb* pcb, t_log* logger);
t_pcb* recibir_pcb(int socket_cliente, t_log* logger);
t_pcb* deserializar_buffer_pcb(t_buffer* buffer, t_log* logger);
void destruir_pcb(t_pcb* pcb);
void destruir_instruccion(t_instruccion *instruccion);
t_registro inicializar_registros();


#endif