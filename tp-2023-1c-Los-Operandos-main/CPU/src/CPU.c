#include "CPU.h"
t_log* logger;
t_config *config;


int pc = 0;
int tam_max_segmento;
int retardo_instruccion;

int conexionConMemoria;
char *ipMemoria;
char *puertoMemoria;
char *valorMemoria;
char *puerto_escucha;

t_temporal* temporizador;

int main(){
	logger = iniciar_logger();

	config = iniciar_config();

	conexionConMemoria= crear_conexion(ipMemoria, puertoMemoria);
	int server_fd = iniciar_servidor(puerto_escucha, "127.0.0.1", logger);

	// espero a kernel

	pthread_t hilo_de_escucha;
	while(1){
		int socket_kernel = esperar_cliente(server_fd);
		pthread_create(&hilo_de_escucha, NULL, (void*)atender_kernel, (void*)socket_kernel);
		pthread_detach(hilo_de_escucha);
	} //escucha infinita 


}

t_log* iniciar_logger(void) {
	t_log *nuevo_logger;
	nuevo_logger = log_create("cfg/CPU.log","cpu_logger", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void) {
	t_config *nuevo_config;
	nuevo_config = config_create("cfg/CPU.config");

	if(nuevo_config == NULL){
	    // ¡No se pudo crear el config!
	    // Terminemos el programa
		log_error(logger,"error al leer el archivo cpu.config");
	}else {

		    ipMemoria = config_get_string_value(nuevo_config,"IP_MEMORIA");
			puertoMemoria = config_get_string_value(nuevo_config,"PUERTO_MEMORIA");
			valorMemoria = config_get_string_value(nuevo_config,"CLAVE_MEMORIA");
			tam_max_segmento = config_get_int_value(nuevo_config,"TAM_MAX_SEGMENTO");
			retardo_instruccion = config_get_int_value(nuevo_config,"RETARDO_INSTRUCCION");
			puerto_escucha = config_get_string_value(nuevo_config, "PUERTO_ESCUCHA");

	}

	return nuevo_config;
}

void atender_kernel(void* conexion)
{
	int conexion_kernel = (int)conexion;
	log_info(logger, "escuchando al kernel");
	t_pcb* pcb = recibir_pcb(conexion_kernel);
	log_info(logger, "recibi pcb");

	temporizador = temporal_create();
	comenzar_ciclo_de_instruccion(pcb, conexion_kernel); 
	destruir_pcb(pcb); 
	close(conexion_kernel);

}
