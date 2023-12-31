#include "kernel.h"

t_list* lista_instrucciones;

t_config *config;
t_log *logger;

t_list *cola_new;
t_list *cola_ready;
t_list *cola_running;
t_list *cola_blocked;
t_list *cola_exit;
t_list* cola_bloqueados_IO;

int server_socket;
int cpu_socket;
int filesystem_socket;
int memoria_socket;

t_kernel_config kernel_config;

pthread_t threadCPU;
pthread_t threadMemoria;
pthread_t threadFileSystem; 

pthread_mutex_t procesosEnSistemaMutex; 

sem_t contador_multiprogramacion;
sem_t sem_nuevos;
sem_t sem_ready;
sem_t sem_bloqueados_fs;
sem_t sem_bloqueados_archivos;
sem_t compactacion;

t_pcb* pcb_a_ejecutar;
t_list* cola_procesos; 
t_dictionary* diccionario_archivos;
t_list* tabla_global_archivos;
pthread_mutex_t procesosBloqueadosFileSystemMutex;
t_list* cola_bloqueados_archivos;
t_list* cola_bloqueados_fs;
pthread_mutex_t puedeIniciarCompactacionMutex;

int main(void) {

	startUp();
	
	if (kernel_config.config == NULL) {
	    log_error(logger, "No se pudo leer el archivo de configuración.");
		return EXIT_FAILURE;	
	}else {
		log_info(logger,"No es null el archivo de config");
	}
	
	hilo_planificador_largo_plazo();
	hilo_planificador_corto_plazo();
	iniciar_file_system(); 

	server_socket = iniciar_servidor(kernel_config.listenPort, kernel_config.kernelIP, logger);

	memoria_socket = crear_conexion(kernel_config.memoryIP,kernel_config.memoryPort);
	log_info(logger, "conecte con memoria");
	send(memoria_socket, "hola, soy kernel", strlen("hola, soy kernel") + 1, 0);
	
	filesystem_socket = crear_conexion(kernel_config.filesystemIP,kernel_config.filesystemPort);
	log_info(logger, "conecte con filesystem");

	pthread_t hilo_de_escucha_consola;
	while(1){
		int socket_consola = esperar_cliente(server_socket);
		pthread_create(&hilo_de_escucha_consola, NULL, (void*)atender_consola, (void*)socket_consola);
		pthread_detach(hilo_de_escucha_consola);
	}

	
	log_destroy(logger);
	config_destroy(config);

	return EXIT_SUCCESS;
}


void startUp(void){


	logger = log_create(LOG_PATH,"kernel_logger", true, LOG_LEVEL_DEBUG);	
	

	kernel_config =	levantar_config();
	iniciar_colas();
	iniciar_semaforos();
	iniciar_recursos();
}

void terminate_kernel(int x)
{
	list_destroy(cola_new);
	list_destroy(cola_ready);
	list_destroy(cola_running);
	list_destroy(cola_blocked);
	list_destroy(cola_exit);


	config_destroy(config);
	if (cpu_socket)
		close(cpu_socket);
	if (memoria_socket)
		close(memoria_socket);
	if (filesystem_socket)
		close(filesystem_socket);

	log_destroy(logger);

	switch (x)
	{
	case 1:
		exit(EXIT_FAILURE);
	case SIGINT:
		exit(EXIT_SUCCESS);
	}
}

