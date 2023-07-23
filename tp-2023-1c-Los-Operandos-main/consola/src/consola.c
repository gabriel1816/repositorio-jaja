#include "consola.h"

#define MAX_LINE_LENGTH 1024 

int kernel_socket;
t_log *logger;
t_console_config console_config;

int main(int argc, char *argv[]){ // Argc es la cantidad de argumentos y argv son los argumentos. Esta forma es un estandar.
  //char *path_configuracion, char *path_pseudocodigo quiero que sean las entradas

  startUp(argv[1]); // Paso como parametro el archivo de configuracion
  if (console_config.config == NULL) {
	    log_error(logger, "No se pudo leer el archivo de configuración.");
		  exit(EXIT_FAILURE);
	}else {
		log_info(logger,"No es null el archivo de config");
	}
	
  // Acá quiero que lea el archivo de pseudocódigo

  FILE *pseudocode_file = fopen(argv[2], "r");
  if (!pseudocode_file) {
    log_error(logger, "No se pudo abrir el archivo de pseudocódigo \n");
    exit(EXIT_FAILURE);
  }


  log_info(logger, "Consola iniciada");

  // parsear archivo pseudocodigo
  t_list *lista_instrucciones = parsear_pseudocodigo(pseudocode_file);


  if (lista_instrucciones == NULL)
  {
      terminar_programa();
      return 0;
  }



  kernel_socket= crear_conexion(console_config.kernelIP, console_config.kernelPort);

  enviar_instrucciones(kernel_socket, lista_instrucciones, logger);

  char mensaje[20];
  recv(kernel_socket, mensaje, sizeof(mensaje), 0);
  log_info(logger, "motivo finalizacion de proceso: %s", mensaje);
  
  // fin del programa
  terminar_programa();
  list_destroy_and_destroy_elements(lista_instrucciones, (void*)destruir_instruccion);

  return EXIT_SUCCESS;

}

void startUp(char *path_configuracion){
	logger = log_create("./cfg/consola.log","CONSOLE", true, LOG_LEVEL_INFO); // Inicializo logger
  console_config.config = config_create(path_configuracion);
  console_config.kernelIP = config_get_string_value(console_config.config, "IP_KERNEL");
	console_config.kernelPort = config_get_string_value(console_config.config, "PUERTO_KERNEL");
}

void terminar_programa() {
		liberar_conexion(kernel_socket);
		log_destroy(logger);
		config_destroy(console_config.config);
}
