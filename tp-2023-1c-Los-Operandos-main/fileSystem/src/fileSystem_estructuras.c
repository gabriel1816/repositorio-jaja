#include "fileSystem.h"

//levanta archivos y crea estructuras de fcb
t_lista_fcb* lista_fcbs[25];

void levantar_estructuras(){

	//abro archivo de suberbloque y me guardo el tamanio y la cant de bloques

	FILE *superbloque_file = fopen(path_superbloque, "r");
  	if (!superbloque_file) {
    	fprintf(stderr, "No se pudo abrir el archivo de suberbloque %s\n", "cfg/suberbloque.txt");
    	exit(EXIT_FAILURE);
  	}
  	char* linea = malloc(MAX_LINE_LENGTH * sizeof(char));
  	if (!linea) {
    	fprintf(stderr, "No se pudo asignar memoria para el archivo de suberbloque\n");
    	exit(EXIT_FAILURE);
  	}
	else{ //leo y asigno los valores a las variables
		fgets(linea, MAX_LINE_LENGTH, superbloque_file);
    	char **lista = string_split(linea, '=');  
		tamanio_bloque = strtol(lista[1], NULL, 10);

		fgets(linea, MAX_LINE_LENGTH, superbloque_file);
		lista = string_split(linea, '=');  
		cant_bloques = strtol(lista[1], NULL, 10);
	}
	fclose(superbloque_file);


  
//levanta el bitmap de bloques 

  	FILE *bitmap_file = fopen(path_bloques, "r");
  	if (!bitmap_file) {
    	fprintf(stderr, "No se pudo abrir el archivo de bloques %s\n", "cfg/bloques.txt");
    	exit(EXIT_FAILURE);
  	}
  	linea = malloc(MAX_LINE_LENGTH * sizeof(char));
  	if (!linea) {
    	fprintf(stderr, "No se pudo asignar memoria para el archivo de bloques\n");
    	exit(EXIT_FAILURE);
  	}

	// abro archivo de bloques

  	FILE *bloques_file = fopen(path_bloques, "r");
  	if (!bloques_file) {
    	fprintf(stderr, "No se pudo abrir el archivo de bloques %s\n", "cfg/bloques.txt");
    	exit(EXIT_FAILURE);
  	}
  	linea = malloc(MAX_LINE_LENGTH * sizeof(char));
  	if (!linea) {
    	fprintf(stderr, "No se pudo asignar memoria para el archivo de bloques\n");
    	exit(EXIT_FAILURE);
  	}

	// recorro el directorio de fcbs y por cada fcb guardo su informacion en una lista de fcbs 
	

	FILE *dir_fcbs = fopen(path_fcb, "r");
	char *nombre_fcb = malloc(MAX_LINE_LENGTH * sizeof(char)); //la linea me lee el nombre del fcb
	while(fgets(nombre_fcb, MAX_LINE_LENGTH, dir_fcbs)){
		t_fcb *fcb;

		FILE *fcbx = fopen(fcb, "r");  // abro el fcb
		char *linea = malloc(MAX_LINE_LENGTH * sizeof(char));

		//nombre
		fgets(linea, MAX_LINE_LENGTH, superbloque_file);
		char **lista0 = string_split(linea, '=');  
		strcpy(fcb->nombre_archivo, lista0[1]);
		
		//tamanio
		fgets(linea, MAX_LINE_LENGTH, fcbx); 
		char **lista1 = string_split(linea, '=');  
		fcb->tam_archivo = strtol(lista1[1], NULL, 10);

		fgets(linea, MAX_LINE_LENGTH, fcbx); 
		char **lista2 = string_split(linea, '=');  
		fcb->puntero_directo = (uint32_t)lista2[1];

		fgets(linea, MAX_LINE_LENGTH, fcbx); 
		char **lista3 = string_split(linea, '=');  
		fcb->puntero_indirecto = (uint32_t)lista3[1];
	
		//*lista_fcbs = agregar_a_lista(lista_fcbs, fcb);
	}

}

t_lista_fcb* agregar_a_Lista(t_lista_fcb* lista, t_fcb* fcb_a_agregar){

	t_lista_fcb* listaAux = malloc(sizeof (t_list));
	listaAux->fcb = fcb_a_agregar;
	listaAux->siguiente_fcb = lista;

	lista = listaAux;
	return lista;
}