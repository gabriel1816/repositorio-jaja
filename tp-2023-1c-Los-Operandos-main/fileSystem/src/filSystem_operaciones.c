#include "fileSystem.h"

bool abrir_archivo(char* nombre_archivo) {
    for(int i = 0; i < list_size(lista_fcbs); i++) {
        t_fcb* nuevo_fcb = list_get(lista_fcbs, i);
        if(strcmp(nuevo_fcb->nombre_archivo, nombre_archivo) == 0) {
            return true; 
        }
    }

    return false;
}

t_fcb* buscar_fcb_por_nombre(t_list* lista_fcbs, char* nombre_archivo) {
    for(int i = 0; i < list_size(lista_fcbs); i++) {
        t_fcb* nuevo_fcb = list_get(lista_fcbs, i);
        if(strcmp(nuevo_fcb->nombre_archivo, nombre_archivo) == 0) {
            return nuevo_fcb; 
        }
    }
}

int crear_archivo(char *nombre) {
    t_fcb *fcb = malloc(sizeof(t_fcb));
    fcb->nombre_archivo = malloc(strlen(nombre) + 1);

    strcpy(fcb->nombre_archivo, nombre);
    fcb->tam_archivo = 0;
    fcb->puntero_directo = 0;
    fcb->puntero_indirecto = 0;
    char *ruta_fcb = malloc(strlen(path_fcb) + strlen(nombre) + 2);
    strcpy(ruta_fcb, path_fcb);
    strcat(ruta_fcb, "/");
    strcat(ruta_fcb, nombre);

    FILE *file_fcb = fopen(ruta_fcb, "w"); // abro el archivo en modo de escritura
    if (file_fcb == NULL) {
        // hubo un error al abrir el archivo
        free(ruta_fcb);
        free(fcb->nombre_archivo);
        free(fcb); 
        return -1; // devuelvo -1 en caso de error
    }

    fprintf(file_fcb, "NOMBRE_ARCHIVO=%s\n", fcb->nombre_archivo);
    fprintf(file_fcb, "TAMANIO_ARCHIVO=%d\n", fcb->tam_archivo);
    fprintf(file_fcb, "PUNTERO_DIRECTO=%u\n", fcb->puntero_directo);
    fprintf(file_fcb, "PUNTERO_INDIRECTO=%u", fcb->puntero_indirecto);

    fclose(file_fcb); // cierro el archivo
    list_add(lista_fcbs, fcb);
    free(ruta_fcb);
    log_info(logger, "Crear Archivo: %s", nombre);
    return 0; // devuelvo 0 siempre porque es exitoso, siempre se puede crear el archivo
}

void actualizarFCB (t_fcb *fcb) {
    char *ruta_fcb = malloc(strlen(path_fcb) + strlen(fcb->nombre_archivo) + 2);
    strcpy(ruta_fcb, path_fcb);
    strcat(ruta_fcb, "/");
    strcat(ruta_fcb, fcb->nombre_archivo);
    t_config *fcb_config = config_create(ruta_fcb);

    config_set_value(fcb_config, "NOMBRE_ARCHIVO", fcb->nombre_archivo);
    config_set_value(fcb_config, "TAMANIO_ARCHIVO", string_itoa(fcb->tam_archivo));
    config_set_value(fcb_config, "PUNTERO_DIRECTO", string_itoa(fcb->puntero_directo));
    config_set_value(fcb_config, "PUNTERO_INDIRECTO", string_itoa(fcb->puntero_indirecto));
    config_save(fcb_config);
}

void truncar_archivo(char *nombre_archivo, int tamanio){
    log_info(logger, "Truncar Archivo: %s - Tamaño: %d", nombre_archivo, tamanio);

    t_fcb* fcb = buscar_fcb_por_nombre(lista_fcbs, nombre_archivo);
    if(fcb->tam_archivo > tamanio) {
       acortar_archivo(fcb, tamanio); 
    }
    else {
        agrandar_archivo(fcb, tamanio);
    }
    actualizarFCB (fcb);
    list_clean(lista_fcbs);
    levantar_fcbs(path_fcb);

}

void acortar_archivo(t_fcb* fcb, uint32_t tamanio_nuevo) {
    
    int bloques_a_liberar = calcular_bloques_a_liberar(tamanio_nuevo, fcb);

    liberar_bloques(fcb, bloques_a_liberar);

    fcb->tam_archivo = tamanio_nuevo;
} 

void agrandar_archivo(t_fcb* fcb, uint32_t tamanio_nuevo) {

    uint32_t bloques_adicionales = calcular_bloques_adicionales(tamanio_nuevo, fcb);

    if (bloques_adicionales > 0)
    {
        
            uint32_t bloque_libre; 
                if(fcb->tam_archivo == 0) { 
                    bloque_libre = buscar_bloque_libre(bitmap);
                    fcb->puntero_directo = bloque_libre;
                    bloque_libre = buscar_bloque_libre(bitmap);  
                    fcb->puntero_indirecto = bloque_libre; 
                    bloques_adicionales--;
                        } 
                    asignar_bloques(fcb, bloques_adicionales);
    }
    fcb->tam_archivo = tamanio_nuevo;
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

void leer_archivo(char *nombre, int puntero_archivo, int cantidad_bytes_para_leer, int direccion_fisica, pid_t pid) {
    t_fcb *fcb = buscar_fcb_por_nombre(lista_fcbs, nombre);
    
    int archivoBloques = open(path_bloques, O_RDWR);

    char* datos_leidos = malloc(cantidad_bytes_para_leer + 1);
    strcpy(datos_leidos, "");
    int bytes_para_leer_de_bloque_actual = 0;
    int puntero_en_bloque = 0;
    int bloque_leido;

    while (cantidad_bytes_para_leer > 0)
    {
        int bloque_actual = ceil((double)puntero_archivo/(double)superbloque->tam_bloque);
        puntero_en_bloque = puntero_archivo%superbloque->tam_bloque;
        if(puntero_en_bloque == 0) {
            bloque_actual += 1;
        }
        if ((superbloque->tam_bloque - puntero_en_bloque) > cantidad_bytes_para_leer) 
        { 
            bytes_para_leer_de_bloque_actual = cantidad_bytes_para_leer; 
            } 
        else { 
            bytes_para_leer_de_bloque_actual = superbloque->tam_bloque - puntero_en_bloque; 
            }
        
        if(bloque_actual == 1) {
            lseek(archivoBloques, (fcb->puntero_directo) * superbloque->tam_bloque + puntero_en_bloque, SEEK_SET);
            log_info(logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d ", nombre, bloque_actual, fcb->puntero_directo);
            char* datitos = malloc(bytes_para_leer_de_bloque_actual);
            ssize_t numBytesLeidos = read(archivoBloques, datitos, bytes_para_leer_de_bloque_actual);
            strcat(datos_leidos, datitos);
            puntero_archivo += bytes_para_leer_de_bloque_actual;
            cantidad_bytes_para_leer -= bytes_para_leer_de_bloque_actual;
            sleep(retardo_acceso_bloque / 1000);
        }
        else {
            log_info(logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: 2 - Bloque File System %d ", nombre, fcb->puntero_indirecto);       
            lseek(archivoBloques, (int)fcb->puntero_indirecto * superbloque->tam_bloque + (bloque_actual - 2) * sizeof(uint32_t), SEEK_SET);
            read(archivoBloques, &bloque_leido, sizeof(uint32_t));
            log_info(logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d ", nombre, bloque_actual+1, bloque_leido);
            lseek(archivoBloques, bloque_leido * superbloque->tam_bloque + puntero_en_bloque, SEEK_SET);
            char* datitos = malloc(bytes_para_leer_de_bloque_actual);
            ssize_t numBytesLeidos = read(archivoBloques, datitos, bytes_para_leer_de_bloque_actual);
            sleep(retardo_acceso_bloque / 1000);
            strcat(datos_leidos, datitos);
            puntero_archivo += bytes_para_leer_de_bloque_actual;
            cantidad_bytes_para_leer -= bytes_para_leer_de_bloque_actual;
            
        }

    }
    sleep(retardo_acceso_bloque / 1000);
    close(archivoBloques);
    
    t_list* lista_parametros = list_create();
    list_add(lista_parametros, datos_leidos);
    
    t_fs_pedido* pedido = malloc(sizeof(t_fs_pedido));
    pedido->instruccion = crear_instruccion(F_READ, lista_parametros);
    pedido->pid = pid;
    pedido->direccion_fisica = direccion_fisica;
    t_buffer *buffer = crear_buffer_para_t_pedido_file_system(pedido);
    t_paquete *paquete = crear_paquete(buffer, 79); // poner código
    enviar_paquete(conexionConMemoria, paquete, logger);
    list_destroy(lista_parametros);

    t_paquete *paquete2 = recibir_paquete(conexionConMemoria, logger);

    destruir_paquete(paquete);
    destruir_paquete(paquete2);


}

void escribir(t_pcb* pcb, int una_conexion) {

    t_fs_pedido* pedido = malloc(sizeof(t_fs_pedido));
    t_instruccion* instruccion = obtener_instruccion(pcb);
    pedido->instruccion = instruccion;
    pedido->pid = pcb->pid;
    pedido->direccion_fisica = pcb->direccion_fisica;
    enviar_pedido_file_system(pedido, conexionConMemoria, logger);
    t_paquete *paquete = get_paquete(conexionConMemoria, logger);
	int offset=0;
    t_instruccion* respuestaMemoria = crear_instruccion_para_el_buffer(paquete->buffer,&offset);
    log_info(logger , "Escribir Archivo: %s - Puntero: %s - Memoria: %d - Tamaño: %s", pedido->instruccion->parametros[0], pedido->instruccion->parametros[1], pedido->direccion_fisica, pedido->instruccion->parametros[2]);
    escribir_archivo(instruccion->parametros[0], atoi(instruccion->parametros[1]), respuestaMemoria->parametros[0], atoi(pedido->instruccion->parametros[2])); 
    destruir_paquete(paquete);
    send(una_conexion, "SI", strlen("SI"), 0);

}

void escribir_archivo(char* nombre, int puntero_archivo, char* datos_a_escribir, int cantidad_bytes_para_escribir) {
    t_fcb* fcb = buscar_fcb_por_nombre(lista_fcbs, nombre);

    if (fcb == NULL)
        return;

    if (fcb->tam_archivo < puntero_archivo + cantidad_bytes_para_escribir) {
        log_error(logger, "No se puede escribir en el archivo %s porque los bytes a escribir superan el tamaño del archivo", nombre);
        free(fcb->nombre_archivo);
        free(fcb);
        return;
    }

    int archivoBloques = open(path_bloques, O_RDWR);
    int bytes_escritos = 0;
    int bytes_para_escribir_en_bloque_actual = 0;
    int bloque_leido;
    int puntero_en_bloque = 0;

    while (cantidad_bytes_para_escribir > 0) {
        int bloque_actual = ceil((double)puntero_archivo / (double)superbloque->tam_bloque);
        puntero_en_bloque  = puntero_archivo % superbloque->tam_bloque;
        if(puntero_en_bloque == 0) {
            bloque_actual += 1;
        }

        if ((superbloque->tam_bloque - puntero_en_bloque) > cantidad_bytes_para_escribir) {
            bytes_para_escribir_en_bloque_actual = cantidad_bytes_para_escribir;
        } else {
            bytes_para_escribir_en_bloque_actual = superbloque->tam_bloque - puntero_en_bloque;
        }

        if (bloque_actual == 1) {
            lseek(archivoBloques, (fcb->puntero_directo) * superbloque->tam_bloque + puntero_en_bloque, SEEK_SET);
            log_info(logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d ", nombre, bloque_actual, fcb->puntero_directo);
            char* datitos = string_substring_from(datos_a_escribir, bytes_escritos);
            write(archivoBloques, datitos, bytes_para_escribir_en_bloque_actual);
            puntero_archivo += bytes_para_escribir_en_bloque_actual;
            bytes_escritos += bytes_para_escribir_en_bloque_actual;
            cantidad_bytes_para_escribir -= bytes_para_escribir_en_bloque_actual;
            sleep(retardo_acceso_bloque/ 1000);
        } else {
            log_info(logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: 2 - Bloque File System %d ", nombre, fcb->puntero_indirecto);       
            lseek(archivoBloques, (int)fcb->puntero_indirecto * superbloque->tam_bloque + (bloque_actual - 2) * sizeof(uint32_t), SEEK_SET);
            read(archivoBloques, &bloque_leido, sizeof(uint32_t));
            log_info(logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d ", nombre, bloque_actual+1, bloque_leido);
            lseek(archivoBloques, bloque_leido * superbloque->tam_bloque + puntero_en_bloque, SEEK_SET);
            char* datitos = string_substring_from(datos_a_escribir, bytes_escritos);
            write(archivoBloques, datitos, bytes_para_escribir_en_bloque_actual);
            sleep(retardo_acceso_bloque / 1000);
            puntero_archivo += bytes_para_escribir_en_bloque_actual;
            bytes_escritos += bytes_para_escribir_en_bloque_actual;
            cantidad_bytes_para_escribir -= bytes_para_escribir_en_bloque_actual;
        }
    }

    sleep(retardo_acceso_bloque / 1000);
    close(archivoBloques);
}

