#include "fileSystem.h"

void iniciar_logger(void) {
	
	logger = log_create(LOG_PATH, "LOGGER FILESYSTEM", true, LOG_LEVEL_INFO);

}

void iniciar_config(void) {
	config = config_create(CONFIG_PATH);

	ipMemoria = config_get_string_value(config,"IP_MEMORIA");
	puertoMemoria = config_get_string_value(config,"PUERTO_MEMORIA");
	valorMemoria = config_get_string_value(config,"CLAVE_MEMORIA");
	puertoEscucha = config_get_string_value(config,"PUERTO_ESCUCHA");
	path_superbloque = config_get_string_value(config,"PATH_SUPERBlOQUE");
	path_bitmap = config_get_string_value(config,"PATH_BITMAP");
	path_bloques = config_get_string_value(config,"PATH_BLOQUES");
	path_fcb = config_get_string_value(config,"PATH_FCB");
	retardo_acceso_bloque = config_get_string_value(config,"RETARDO_ACESO_BLOQUE");
	tam_max_segmento = config_get_int_value(config,"TAM_MAX_SEGMENTO");
	ipFs = config_get_string_value(config,"IP_FILESYSTEM");

}

void terminar_programa(int conexion, t_log *logger, t_config *config) {
		liberar_conexion(conexion);
		log_destroy(logger);
		config_destroy(config);
}

int cant_bloques_nec(int tam_archivo){
	return tamanio_bloque / tam_archivo;
}

void levantar_estructuras(){

	lista_fcbs = list_create();
	//..........
}