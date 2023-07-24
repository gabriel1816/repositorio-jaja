#include "memoria.h"


t_log* logger;
int memoria_libre;
t_list* tablas_segmentos;
t_segmento* segmento_cero;
int memoria_libre;
void* memoria_fisica;
t_list* lista_huecos;
t_algoritmo algoritmo;

char*  puerto;
int  *tamanio_memoria;
int  *tamanio_segmento_0;
int  *cantidad_segmentos;
int  *retardo_memoria;
int  *retardo_compactacion;
char *algoritmo_asignacion;
char* ip_memoria;

pthread_t hilo_de_escucha_kernel, hilo_de_escucha_cpu, hilo_de_escucha_fs;


int main(void) {

	printf("buenas");
	logger = log_create("./cfg/memoria.log", "Servidor", 1, LOG_LEVEL_DEBUG);
    levantar_config("./cfg/memoria.config");

    // creo tabla 

    //crear_estructuras();
	log_info(logger, "cree estructuras");

    // Conexiones

    int server_socket = iniciar_servidor(puerto, ip_memoria, logger);
    
    pthread_t hilo_de_escucha_kernel;
	while(1){
	    int socket_kernel = esperar_cliente(server_socket);
	    pthread_create(&hilo_de_escucha_kernel, NULL, (void*)atender_kernel, (void*)socket_kernel);
	    pthread_detach(hilo_de_escucha_kernel);
	} //escucho kernel

    pthread_t hilo_de_escucha_cpu;
	while(1){
	    int socket_CPU = esperar_cliente(server_socket);
	    pthread_create(&hilo_de_escucha_cpu, NULL, (void*)atender_cpu, (void*)socket_CPU);
	    pthread_detach(hilo_de_escucha_cpu);
	} //escucho cpu

    pthread_t hilo_de_escucha_fs;
    while(1){
	    int socket_fs = esperar_cliente(server_socket);
	    pthread_create(&hilo_de_escucha_fs, NULL, (void*)atender_fs, (void*)socket_fs);
	    pthread_detach(hilo_de_escucha_fs);
	} // escucho filesystem
   
}