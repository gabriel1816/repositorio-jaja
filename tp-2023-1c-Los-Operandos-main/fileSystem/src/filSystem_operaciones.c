#include "fileSystem.h"
#include "fileSystem_estructuras.c"


// funciones de archivos
bool abrir_archivo(char* nombre_archivo)
{ //si devuelve false es porque no existe el archivo
	bool encontrado = true;
	//abro el archivo directorio de fcbs

    FILE *archivo = fopen(strcat("FCBs/", nombre_archivo), "r");
    if(!archivo){
        encontrado = false;
    }    

    fclose(archivo);
	return encontrado;
}

bool crear_archivo(char *nombreArchivo){ //siempre devuelve true porque siempre se va a poder crear un archivo
	//abro el archivo de directorios y escribo el nombre del fcb 
    char *dato1 = strcat("NOMBRE_ARCHIVO=", nombreArchivo);
    char *dato2 = "TAMANIO_ARCHIVO=0";
    char *dato3 = "PUNTERO_DIRECTO=0";
    char *dato4 = "PUNTERO_INDIRECTO=0";

    FILE *nuevoArchivo = fopen(strcat("FCBs/", nombreArchivo), "r");
    
    fwrite(dato1, sizeof(char*), 1, nuevoArchivo);
    fwrite(dato2, sizeof(char*), 1, nuevoArchivo);
    fwrite(dato3, sizeof(char*), 1, nuevoArchivo);
    fwrite(dato4, sizeof(char*), 1, nuevoArchivo); 

    fclose(nuevoArchivo);

    t_fcb *fcb_nuevo;
    fcb_nuevo->nombre_archivo = nombreArchivo;
    fcb_nuevo->tam_archivo = 0;
    fcb_nuevo->puntero_directo = 0;
    fcb_nuevo->puntero_indirecto = 0;

    list_add(lista_fcbs, fcb_nuevo);

	return true;
}

void truncar_archivo(char *nombre_archivo, int tamanio){
    //obtengo el tamanio del archivo
    t_fcb* fcbArchivo = obtenerFcb(nombre_archivo);
    int cant_bloques_ocupados = cant_bloques_nec(fcbArchivo->tam_archivo);
    int nueva_cant_bloques = cant_bloques_nec(tamanio);
    
    if(tamanio < fcbArchivo->tam_archivo){
        int bloques_a_borrar = cant_bloques_ocupados - nueva_cant_bloques;
        borrar_bloques(cant_bloques_ocupados, bloques_a_borrar, fcbArchivo);
        
    }
    else{
        int bloques_a_agregar = nueva_cant_bloques - cant_bloques_ocupados;
        agregar_bloques(cant_bloques_ocupados, bloques_a_agregar, fcbArchivo);
        //si ocupo otro archivo actualizo el bitmap
    }

     //actualizo el tamanio en el fcb
    fcbArchivo->tam_archivo = tamanio;
}


void leer_archivo(char *path_archivo, void* info_a_leer, int cantBytes){
	//voy a buscar el fcb
    t_fcb* fcbArchivo = obtenerFcb(path_archivo);
    // de ahi saco el puntero y el tamanio 
    int tam_archivo = fcbArchivo->tam_archivo;
	uint32_t puntero_directo = fcbArchivo->puntero_directo;
    int cant_bytes_leidos = 0;
    int num_bloque_logico;
    int num_bloque_fisico;
    int offset;
    int posicion;
    int cant_bytes_a_leer;
    int tam_a_leer;
   
    // con el puntero voy al de bloques y busco el primer bloque y hago un while hasta que los bytes leidos sean igual al tamanio algo asi y voy guardando todo en un string
    while(cant_bytes_leidos < cantBytes){
        int tam_a_leer_max = tamanio_bloque - (puntero_directo % tamanio_bloque);
		//int tam_a_leer = min(cantBytes-cant_bytes_leidos, tam_a_leer_max);
        //^^^^ if

        num_bloque_logico = puntero_directo/tamanio_bloque; //numero de bloque al que apunta el puntero
        num_bloque_fisico = bloqueLogicoAFisico(fcbArchivo, num_bloque_logico);
        offset = puntero_directo % tamanio_bloque;
        posicion = num_bloque_fisico*tamanio_bloque + offset; //la posicion del puntero mas el offset
        cant_bytes_a_leer = cantBytes - cant_bytes_leidos;
        
        //leo el archivo
        FILE* archivo_bloques = fopen(path_bloques, "r+b");
	    fseek(archivo_bloques, posicion, SEEK_SET);
	    fread(info_a_leer + cant_bytes_a_leer, tam_a_leer,1, archivo_bloques); //lee y va guardando en la variable info_a_leer
	    fclose(archivo_bloques);

        cant_bytes_leidos += cant_bytes_a_leer;
	    puntero_directo += cant_bytes_a_leer;
    }
    
}

void escribir_archivo(char *path_archivo, char info_a_escribir, int cantBytes){
    t_fcb* fcbArchivo = obtenerFcb(path_archivo);
    int tam_archivo = fcbArchivo->tam_archivo;
	uint32_t puntero_directo = fcbArchivo->puntero_directo;
    int cant_bytes_escritos = 0;
    int num_bloque_logico;
    int num_bloque_fisico;
    int offset;
    int posicion;
    int cant_bytes_a_escribir;
    
    while(cant_bytes_escritos  < cantBytes){ 
        num_bloque_logico = puntero_directo/tamanio_bloque; //numero de bloque al que apunta el puntero
        num_bloque_fisico = bloqueLogicoAFisico(fcbArchivo, num_bloque_logico);
        offset = puntero_directo % tamanio_bloque;
        posicion = num_bloque_fisico *tamanio_bloque + offset; //la posicion del puntero mas el offset
        cant_bytes_a_escribir = cantBytes - cant_bytes_escritos;

    //escribo el archivo de bloques
        FILE* archivo_bloques = fopen(path_bloques, "r+b");
	    fseek(archivo_bloques, posicion, SEEK_SET);
	    fwrite(info_a_escribir, cant_bytes_a_escribir,1, archivo_bloques);
	    fclose(archivo_bloques);

        cant_bytes_escritos += cant_bytes_a_escribir;
	    puntero_directo += cant_bytes_a_escribir;
    }
	
}

t_fcb* obtenerFcb(char *nombre_archivo){
  t_fcb* fcb;
  int tamanio = sizeof(lista_fcbs) / sizeof(lista_fcbs[0]);

    for(int i = 0; i  < tamanio; i++){  // no se como obtener el tamanio de la lista
      
        if(lista_fcbs[i]->fcb->nombre_archivo == nombre_archivo){
            *fcb = *lista_fcbs[i]->fcb;
        }
    }
    
    return fcb;
}

int bloqueLogicoAFisico(t_fcb* fcb, int num_bloque){
	int bloque_fisico;
	if(num_bloque == 0){
		return fcb->puntero_directo;
	}else{
		int posicion = fcb->puntero_indirecto * tamanio_bloque + (num_bloque - 1)*sizeof(uint32_t);
		//leerArchivoBloques((void*)&bloque_fisico, posicion, sizeof(uint32_t));
		return bloque_fisico;
	}
}

void borrar_bloques(int bloques_ocupados, int cant_bloques_a_borrar, t_fcb *fcb){
   if(bloques_ocupados == 1){
       bitarray_clean_bit(bit_array, fcb->puntero_directo); //borro el valor 
    }
    else{
        for(int i = 0; i < cant_bloques_a_borrar; i++){
            desasignar_bloque(fcb->puntero_indirecto, cant_bloques_a_borrar -1);
        }
    }
}

void agregar_bloques(int bloques_ocupados, int cant_bloques_a_agregar, t_fcb *fcb){
    if(bloques_ocupados == 0){
        fcb->puntero_directo = asignar_bloque_libre();
        cant_bloques_a_agregar -= 1; 
    }
    else{
        for(int i = 0; i < cant_bloques_a_agregar; i++){
            fcb->puntero_indirecto = asignar_bloque_libre();
        }
    }
    
}

uint32_t asignar_bloque_libre(){
	bool bloque_libre = false;
    uint32_t bloque;
	int num_linea; //cada linea del archivo de bitmap me representa un bloque

    FILE *bitmap = fopen(path_bloques, "r");
    int i = 0;
	while(bloque_libre == false){//recorro el archivo bitmp y busco el primer bloque libre
        if(bitarray_test_bit(bit_array, i) == 0){
           	bloque_libre == true;
			bloque = (uint32_t)i;
            bitarray_clean_bit(bit_array, i); //borro el valor actual
            fwrite(bit_array->bitarray, 1, bit_array->size, bitmap);//actualizo el valor de 0 a 1
            
        }
		i ++;
    }

    fclose(bitmap);
		
	return bloque;
}

void desasignar_bloque(uint32_t puntero, int bloques_a_borrar){
   int i = 0;
   while(i < bloques_a_borrar){

     bitarray_clean_bit(bit_array, puntero); 

     i += 1;
   }
}