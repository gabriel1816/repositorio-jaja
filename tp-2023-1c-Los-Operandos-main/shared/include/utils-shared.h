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

#define PUERTO "4444"
#define INITIAL_STREAM_SIZE 64

typedef struct pQueue
{
	t_queue *lib_queue;
	pthread_mutex_t mutex;
	sem_t sem;
} t_pQueue;

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
	int codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct {
    t_list* segmentos;
	int pid;
} t_tabla_segmentos;



typedef struct{
    int id;
    pid_t pid;
    void* base;
    void* limite;
    int tamanio;
    bool libre;
} t_segmento;

extern t_log* logger;

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
	int cant_parametros;
	int param_length1;
	int param_length2;
	int param_length3;
	int param_length4;
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
	//Falta tabla de segmentos y tabla de archivos abiertos
	//Agregar prioridad?
}t_pcb;

typedef struct 
{
	t_instruccion* instruccion;
	t_list* siguienteInstruc;
}instr;

typedef struct
{
	int numero_segmento;
	int desplazamiento;
}t_direc_fisica;

typedef struct t_stream_buffer {
    uint32_t offset;
    size_t malloc_size;
    char* stream;
} t_stream_buffer;

typedef struct t_packet {
	uint8_t header;
	t_stream_buffer *payload;
} t_packet;



bool catch_syscall_err(int code);
int accept_client(int server_socket);
void server_listen(int server_socket, void* (*client_handler)(void*));
t_packet* create_packet(uint8_t header, size_t size);
void packet_destroy(t_packet *packet);
int create_server(char *server_port);
// int receive_wrapper(int socket, void *dest, size_t size);
// bool socket_receive(int socket, void *dest, size_t size);
int connect_to(char *server_ip, char *server_port);
uint8_t socket_receive_header(int socket);
t_packet *socket_receive_packet(int socket);
bool socket_retry_packet(int socket, t_packet **packet);
// int send_wrapper(int socket, void *buffer, size_t size);
// void socket_send(int socket, void *source, size_t size);
void socket_send_header(int socket, uint8_t header);
void socket_send_packet(int socket, t_packet *packet);

int mi_funcion_compartida();

int crear_conexion(char* ip, char* puerto);
t_paquete* crear_paquete(t_buffer* buffer, int codigo_op);
t_paquete* crear_super_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void* recibir_buffer(int* size, int socket_cliente);
void destruir_buffer(t_buffer* buffer);
t_paquete *package_recv(int socket, t_log *logger);

t_stream_buffer* create_stream(size_t size);
int iniciar_servidor(char *puerto, char* ip, t_log* logger);
void stream_destroy(t_stream_buffer *stream);
void stream_add(t_stream_buffer *stream, void *source, size_t size);
void stream_add_UINT32P(t_stream_buffer *stream, void *source);
void stream_add_UINT32(t_stream_buffer *stream, uint32_t value);
void stream_add_STRINGP(t_stream_buffer *stream, void *source);
void stream_add_STRING(t_stream_buffer *stream, char *source);
void stream_add_LIST(t_stream_buffer* stream, t_list* source, void(*stream_add_ELEM_P)(t_stream_buffer*, void*));
void stream_take(t_stream_buffer *stream, void **dest, size_t size);
void stream_take_UINT32P(t_stream_buffer *stream, void **dest);
uint32_t stream_take_UINT32(t_stream_buffer *stream);
void stream_take_STRINGP(t_stream_buffer *stream, void **dest);
char* stream_take_STRING(t_stream_buffer *stream);
void stream_take_LISTP(t_stream_buffer* stream, t_list** source, void(*stream_take_ELEMP)(t_stream_buffer*, void**));
t_list* stream_take_LIST(t_stream_buffer* stream, void(*stream_take_ELEMP)(t_stream_buffer*, void**));


t_list *deserializar_lista_instrucciones( t_buffer *buffer);
t_instruccion *crear_instruccion_para_el_buffer(t_buffer *buffer, int *offset);
t_buffer *crear_buffer_para_t_instruccion(t_instruccion *instruccion);
void enviar_instrucciones(int socket, t_list *lista_instrucciones, t_log *logger);
t_buffer *crear_buffer__para_t_lista_instrucciones(t_list *lista_instrucciones);
int espacio_de_array_parametros(t_instruccion *instruccion);
void destruir_instruccion(t_instruccion* instruccion);


void enviar_pcb(t_pcb* pcb, int socket, t_log* logger);
t_pcb* crear_pcb(t_list* instrucciones, int conexion);
t_registro inicializar_registro();
t_buffer* crear_buffer_pcb(t_pcb* pcb, t_log* logger);
t_pcb* recibir_pcb(int socket_cliente);
t_pcb* deserializar_buffer_pcb(t_buffer* buffer);
void destruir_pcb(t_pcb* pcb);

#endif