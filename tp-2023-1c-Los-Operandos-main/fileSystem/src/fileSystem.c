#include "fileSystem.h"

#define MAX_LINE_LENGTH 1024 


t_log* logger;
t_config *config;
int conexionConMemoria;
char *ipMemoria;
char *puertoMemoria;
char *valorMemoria;
char *puertoEscucha;
char *path_superbloque;
char *path_bitmap;
char *path_bloques;
char *path_fcb;
char *retardo_acceso_bloque;
int tamanio_bloque;
int cant_bloques; 
int tam_max_segmento;
char* ipFs;

t_list* lista_fcbs;

int main(void) {

	iniciar_logger();
	
	iniciar_config();
	//conecto con memoria
	conexionConMemoria= crear_conexion(ipMemoria, puertoMemoria);
	send(conexionConMemoria, "hola, soy filesystem", strlen("hola, soy filesystem") + 1, 0);
	log_info(logger, "conecte con memoria");

	//levantar_estructuras();

	//espero al kernel 
	int socket_servidor = iniciar_servidor(puertoEscucha, ipFs, logger);
	pthread_t hilo_de_escucha_filesystem;
	while(1){
		log_info(logger, "Servidor listo para recibir al cliente");
		int socket_kernel = esperar_cliente(socket_servidor);
		pthread_create(&hilo_de_escucha_filesystem, NULL, (void*)atender_kernel, (void*)socket_kernel);
		pthread_detach(hilo_de_escucha_filesystem);
	}


	terminar_programa(conexionConMemoria, logger, config);
	
	
	return EXIT_SUCCESS;
}

