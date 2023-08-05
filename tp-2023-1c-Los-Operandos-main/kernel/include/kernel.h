#ifndef KERNEL_H_
#define KERNEL_H_
#define CONFIG_PATH "./cfg/kernel.config"
#define LOG_PATH "./cfg/kernel.log"


#include <commons/config.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include "utils-shared.h"
#include <pthread.h>
#include <commons/log.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include <unistd.h>
#include <netdb.h>
#include <assert.h>

//************* ESTRUCTURAS *************//
typedef struct{
	t_list* cola_bloqueados;
	int instancias;
}t_recurso; 

typedef struct
{
	t_config *config;
	char *memoryIP;
	char *memoryPort;
    char *filesystemIP;
    char *filesystemPort;
	char *cpuIP;
	char *cpuPort;
	char *listenPort;
	char *schedulerAlgorithm;
	double initialEstimate;
	double alpha;
	int multiprogrammingLevel;
    char** recursos;
    char** instancias_recursos; 
	char* kernelIP;

} t_kernel_config;

typedef struct {
    t_recurso *archivo;
    char* nombre_archivo;
    int puntero;
} archivo_abierto;

extern t_kernel_config kernel_config;

extern pthread_t threadCPU;
extern pthread_t threadMemoria;
extern pthread_t threadFileSystem; 


//************* VARIABLES  *************//
//************* GLOBALES   *************//

extern t_config *config;

extern t_log *logger;

extern t_list *cola_new;
extern t_list *cola_ready;
extern t_list *cola_running;
extern t_list *cola_blocked;
extern t_list* cola_bloqueados_IO;

extern int server_socket;
extern int cpu_socket;
extern int filesystem_socket;
extern int memoria_socket;
extern t_list* lista_instrucciones;
extern t_pcb* pcb_a_ejecutar;

extern pthread_mutex_t mutex_cola_nuevos;
extern pthread_mutex_t mutex_cola_listos;
extern pthread_mutex_t mutex_cola_bloqueados;
extern pthread_mutex_t mutex_pcb_a_ejecutar;
extern pthread_mutex_t mutex_cambio_estado;
extern pthread_mutex_t procesosEnSistemaMutex; 
extern pthread_mutex_t procesosBloqueadosFileSystemMutex;
extern pthread_mutex_t puedeIniciarCompactacionMutex;
extern sem_t contador_multiprogramacion;
extern sem_t sem_nuevos;
extern sem_t sem_ready;
extern sem_t sem_bloqueados_fs;
extern sem_t compactacion;

extern t_dictionary* diccionario_recursos;
extern t_dictionary* diccionario_archivos;
extern t_list* tabla_global_archivos;
extern t_list* cola_recursos_bloqueados;
extern t_list* cola_procesos;
extern t_list* cola_bloqueados_archivos;
extern t_list* cola_bloqueados_fs;



//************* ---------------  *************//
void startUp(void);
t_kernel_config levantar_config();
void atender_consola(void* conexion);
void atender_cpu(void* conexion);
void atender_memoria(void* conexion);
void atender_fs(void* conexion);
void iniciar_colas();
void iniciar_semaforos();
void iniciar_recursos();
void loggear_cola_listos();

//**************    MEMORIA    **************//

void solicitud_memoria(pid_t pid, t_estado solicitud);
void crear_segmento(t_instruccion* instruccion, t_pcb* pcb);
void eliminar_segmento(t_instruccion* instruccion, t_pcb* pcb);
void recibir_creacion(t_pcb* pcb, t_instruccion* instruccion_pedido);
void actualizar_tabla(t_paquete* paquete, t_pcb* pcb);

//************* INSTRUCCIONES  *************//

void hilo_planificador_corto_plazo();
void hilo_planificador_largo_plazo();
void inicializar_planificador_corto();
void inicializar_planificador_largo();
void ejecutar_con_fifo();
void ejecutar(t_pcb* pcb);
void poner_en_ejecucion(t_pcb* pcb);
void ejecutar_hrrn();
void parar_tiempo();
void reiniciar_tiempos();
bool mayor_response_ratio(t_pcb* pcb1, t_pcb* pcb2);
double response_ratio(t_pcb* pcb);
void actualizar_estimado_rafaga(t_pcb* pcb);
t_pcb* sacar_pcb_de_ejecucion();
void recibir_pcb_de_cpu(int cpu_socket);
void terminar_proceso(t_pcb* pcb_actualizado, char* motivo);
void ejecutar_IO(t_pcb* pcb, t_instruccion* instruccion);
void pasar_a_IO(int tiempo);
void agregar_bloqueados_IO(t_pcb* pcb);
t_pcb* sacar_de_bloqueado_IO();
void ejecutar_signal(t_pcb* pcb, t_instruccion* instruccion);
void ejecutar_wait(t_pcb* pcb, t_instruccion* instruccion);
void cola_recursos_bloqueados_sacar(t_pcb* pcb);
void cola_recursos_bloqueados_agregar(t_pcb* pcb);
void cambiar_estado(t_pcb* pcb, t_estado nuevo_estado);
char* pasar_a_string(t_estado estado);
void actualizar_procesos_recibidos() ;
void actualizar_tabla(t_paquete* paquete, t_pcb* pcb);
void recibir_creacion(t_pcb* pcb, t_instruccion* instruccion_pedido);
void eliminar_segmento(t_instruccion* instruccion, t_pcb* pcb);
void crear_segmento(t_instruccion* instruccion, t_pcb* pcb);
bool mismo_pcb(t_pcb* pcb);

//*************** LISTAS  ***************//
void agregar_a_listos(t_pcb* pcb);
t_pcb* sacar_de_listos();
void agregar_a_bloqueados(t_pcb* pcb);
t_pcb* sacar_de_bloqueados();
void agregar_a_nuevos(t_pcb* pcb);
t_pcb* sacar_de_nuevos();
t_pcb* buscar_pcb(pid_t* pid);
void sacar_de_cola_procesos(t_pcb* proceso);
void agregar_a_cola_procesos(t_pcb* proceso);


bool solicitar_apertura(t_pcb* pcb);
void solicitar_creacion(t_instruccion* instruccion);
void solicitar_truncamiento(t_pcb* pcb);
void leer(t_instruccion* instruccion, t_pcb* pcb);
void escribir(t_instruccion* instruccion, t_pcb* pcb);


//*************** TABLA ARCHIVOS  ***************//

void agregar_a_tabla_global(char* NombreArchivo);
void agregar_a_tabla_proceso(t_pcb* pcb, char* Nombrearchivo);
bool esta_en_tabla(char* Nombrearchivo);
void sacar_de_tabla_proceso(t_pcb* pcb, char* archivo);
void sacar_de_tabla_global(t_pcb* pcb_actualizado, char* archivo);
void eliminar_archivo_por_nombre(t_list* lista, char* nombre);
void actualizar_puntero_tabla_global(char* nombre_archivo, char* puntero);
void desbloquear_proceso(t_pcb* pcb, char* key_archivo); 
bool hay_procesos_en_espera(t_pcb* pcb, char* key_archivo);
void atender_signal_archivo(t_pcb* pcb, char* key_archivo);
void atender_wait_archivo(t_pcb* pcb, char* key_archivo);
void agregar_pcb_cola_bloqueados_RecursoArchivo(t_pcb* pcb); 
void sacar_pcb_cola_bloqueados_RecursoArchivo(t_pcb* pcb);
t_recurso* crear_recurso(int instancias);
archivo_abierto* buscar_archivo(char* archivo_a_sacar);
void agregar_pcb_en_cola_bloqueados_FileSystem(t_pcb* pcb);
t_pcb* sacar_pcb_cola_bloqueados_FileSystem();
void iniciar_file_system();
void file_system(); 
void set_puede_iniciar_compactacion(bool estado);



#endif /* KERNEL_H_ */