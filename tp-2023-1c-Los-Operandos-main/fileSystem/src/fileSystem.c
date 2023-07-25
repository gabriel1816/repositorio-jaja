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
t_lista_fcb* lista_fcbs[25];

int main(void) {

	logger = iniciar_logger();

	config = iniciar_config();
	if (config == NULL) {
	    // ¡No se pudo crear el config!
	    // Terminemos el programa
		printf("error al leer el archivo fileSystem.config");
	}

	else {

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

	}

	 conexionConMemoria= crear_conexion(ipMemoria, puertoMemoria);

	//abro todos los archivos y creo estructuras necesarias
	//levantar_estructuras();

	log_info(logger, "levante");

	 //enviar_mensaje(valorMemoria, conexionConMemoria);

	 //terminar_programa(conexionConMemoria, logger, config);
	 log_info(logger, "termine");

	///////// como servidor -- se conecta con el kernel
	int server_fd = iniciar_servidor("4445", ipMemoria, logger);
	
	log_info(logger, "Servidor listo para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);// aca se deberia conectar con el kernel

	
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			log_info(logger, "recibio mansaje de kernel"); 
			break;
		case PAQUETE:
		// aca el kernel le manda las instrucciones de abrir archivo y eso
		t_paquete *paquete = recibir_paquete(cliente_fd, logger); //recibo el paquete
		t_instruccion *instruccion =  crear_instruccion_para_el_buffer(paquete->buffer, 0);
		atender_instruccion(instruccion, cliente_fd);
		break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;

}

t_log* iniciar_logger(void) {
	t_log *nuevo_logger;
	nuevo_logger = log_create("./cfg/fileSystem.log","SOtp", true, LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void) {
	t_config *nuevo_config;
	nuevo_config = config_create("./cfg/fileSystem.config");

	return nuevo_config;
}

void terminar_programa(int conexion, t_log *logger, t_config *config) {
		liberar_conexion(conexion);
		log_destroy(logger);
		config_destroy(config);
}

int cant_bloques_nec(int tam_archivo){
	return tamanio_bloque / tam_archivo;
}

void atender_instruccion(t_instruccion *instruccion, int socket_kernel){
	switch(instruccion->identificador){
		t_buffer *buffer = malloc(sizeof(t_buffer));
		t_paquete *paquete;
		char *mensaje;
		case F_OPEN: 
			bool abrioElArchivo = abrir_archivo(instruccion->parametros[0]);
			if(abrioElArchivo){
				mensaje = "se pudo abrir el archivo";
			}
			else{
				mensaje = "No se pudo abrir el archivo, el archivo no existe";
			}
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));
			enviar_paquete(socket_kernel, paquete, logger);

		case F_READ: 
			t_direc_fisica *direcFisica_lectura = traducir_direccion(instruccion->parametros[1]);
			char* info_a_leer = malloc(instruccion->parametros[2] + 1);
			
			leer_archivo(instruccion->parametros[0], info_a_leer ,instruccion->parametros[2]);
			
			//le mando a memoria lo que se leyo
			mandar_a_memoria(info_a_leer, direcFisica_lectura, conexionConMemoria, instruccion);

			//le mando a kernel que se leyo el archivo
			mensaje = "se leyo el archivo";
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));
			enviar_paquete(socket_kernel, paquete, logger);

		case F_WRITE: 
		//le solicita a memoria la info que se encuentra a partir de la direc fisica
			t_direc_fisica *direcFisica_escritura = traducir_direccion(instruccion->parametros[1]);
			char datos_para_escribir = pedir_a_memoria(direcFisica_escritura, conexionConMemoria, instruccion);
			
			escribir_archivo(instruccion->parametros[0],datos_para_escribir, instruccion->parametros[2]);
			
			//le mando a kernel que se escribio el archivo
			mensaje = "se escribio el archivo";
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));
			enviar_paquete(socket_kernel, paquete, logger);

		case F_CLOSE:
		t_fcb *fcb_eliminar = obtenerFcb(instruccion->parametros[0]);
		//remover_de_lista(lista_fcbs, fcb_eliminar);//elimino de la lista de fcbs el fcb de ese arch 

		// eimino la entrada del directorio

		case F_TRUNCATE:
			//truncar_archivo(instruccion->parametros[0]);
			mensaje = "se modifico el tamanio del archivo";
			paquete = crear_paquete(buffer, instruccion->identificador);
			agregar_a_paquete(paquete, mensaje, sizeof(char*));

			enviar_paquete(socket_kernel, paquete, logger);

		case F_SEEK:
	}
}

t_direc_fisica* traducir_direccion(int direc_logica)
{
    t_direc_fisica* direc_fisica;
    direc_fisica->desplazamiento = direc_logica % tam_max_segmento;
	direc_fisica->numero_segmento = floor(direc_logica / tam_max_segmento);
    return direc_fisica;
}

char pedir_a_memoria(t_direc_fisica* direccion, int conexionConMemoria, t_instruccion *instruccion){
	t_buffer *buffer = malloc(sizeof(t_buffer));
	t_paquete* paquete = crear_paquete(buffer, instruccion->identificador);
	agregar_a_paquete(paquete, (void*)direccion, sizeof(t_direc_fisica));

	enviar_paquete(conexionConMemoria, paquete, logger);

	eliminar_paquete(paquete);

	// recibimos valor de memoria
	recv(conexionConMemoria, &(paquete->codigo_operacion), sizeof(uint8_t), 0);

	// deserializamos paquete
	char valor;
	void* stream = paquete->buffer->stream;
	memcpy(&(valor), stream, sizeof(char));

	return valor;
}

void mandar_a_memoria(char* valor, t_direc_fisica* direccion, int socketMemoria, t_instruccion *instruccion){

	t_buffer *buffer = malloc(sizeof(t_buffer));
	t_paquete* paquete = crear_paquete(buffer, instruccion->identificador);
	agregar_a_paquete(paquete, (void*)direccion, sizeof(t_direc_fisica));
	agregar_a_paquete(paquete, valor, strlen(valor));
	enviar_paquete(socketMemoria, paquete, logger);


	eliminar_paquete(paquete);

}

// Falta adecuarlo con las variables de memoria
/*
bool memory_handshake(int memory_socket)
{
	pthread_mutex_lock(&mutex_log);
	log_info(logger, " Petición de handshake de Memoria recibida");
	pthread_mutex_unlock(&mutex_log);

	t_packet *mem_data = create_packet(TABLE_INFO_TO_CPU, INITIAL_STREAM_SIZE);
	stream_add_UINT32(mem_data->payload, config->pageSize);
	stream_add_UINT32(mem_data->payload, config->entriesPerTable);
	socket_send_packet(cpu_socket, mem_data);
	packet_destroy(mem_data);

	pthread_mutex_lock(&mutex_log);
	log_info(logger, "Información relevante transmitida a CPU");
	pthread_mutex_unlock(&mutex_log);

	return false;
}

	int memory_handshake_listener()
	{

		int client_socket = accept_client(server_fd);
		bool serve = true;
		while (serve)
		{
			uint8_t header = socket_receive_header(cliente_fd);
			if (header == MEM_HANDSHAKE)
			{
				serve = memory_handshake(client_socket);
			}
		}
		return client_socket;
	}
*/
