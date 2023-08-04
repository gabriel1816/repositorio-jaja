#include "fileSystem.h"

void iniciar_logger(void) {
	
	logger = log_create(LOG_PATH, "LOGGER FILESYSTEM", true, LOG_LEVEL_INFO);

}

void iniciar_config(void) {
	config = config_create(CONFIG_PATH);

	ipMemoria = config_get_string_value(config,"IP_MEMORIA");
	puertoMemoria = config_get_string_value(config,"PUERTO_MEMORIA");
	puertoEscucha = config_get_string_value(config,"PUERTO_ESCUCHA");
	path_superbloque = config_get_string_value(config,"PATH_SUPERBlOQUE");
	path_bitmap = config_get_string_value(config,"PATH_BITMAP");
	path_bloques = config_get_string_value(config,"PATH_BLOQUES");
	path_fcb = config_get_string_value(config,"PATH_FCB");
	retardo_acceso_bloque = config_get_int_value(config,"RETARDO_ACESO_BLOQUE");
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

	levantar_superbloque();
	levantar_bitmap();
	levantar_bloques();
	levantar_fcbs();
}

void levantar_superbloque() {
	t_config* superbloque_config = config_create(path_superbloque);
	superbloque = malloc(sizeof(t_superbloque));
	superbloque->cantidad_bloques = config_get_int_value(superbloque_config,"BLOCK_COUNT");
	superbloque->tam_bloque = config_get_int_value(superbloque_config,"BLOCK_SIZE");
	config_destroy(superbloque_config); 
}
void levantar_bitmap() {
	int tamano_bitmap = ceil(superbloque->cantidad_bloques / 8); 

    int archivo_bitmap = open(path_bitmap, O_CREAT | O_RDWR, 0644);
    ftruncate(archivo_bitmap, sizeof(t_superbloque) + tamano_bitmap);

    void *mmapBitmap = mmap(NULL, sizeof(t_superbloque) + tamano_bitmap, PROT_READ | PROT_WRITE, MAP_SHARED, archivo_bitmap, 0);
    bitmap = bitarray_create_with_mode(mmapBitmap + sizeof(t_superbloque), tamano_bitmap, LSB_FIRST);

    if (lseek(archivo_bitmap, 0, SEEK_END) == 0) { 
        for(int i = 0; i < bitarray_get_max_bit(bitmap); i++) {
            bitarray_clean_bit(bitmap, i); 
        }
    }

    msync(mmapBitmap, sizeof(t_superbloque) + tamano_bitmap, MS_SYNC);

    close(archivo_bitmap);

}

void levantar_bloques() {

	int archivoBloques = open(path_bloques, O_CREAT | O_RDWR, 0644);
    int tamanioArchivoBloques = superbloque->tam_bloque * superbloque->cantidad_bloques;
    ftruncate(archivoBloques, tamanioArchivoBloques);
    close(archivoBloques);
}

void levantar_fcbs() {

	lista_fcbs = list_create();
    DIR *dir = opendir(path_fcb);
    struct dirent *ent;

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (ent->d_type == DT_REG)
            { 
                char ruta_completa[100];  
                sprintf(ruta_completa, "%s/%s", path_fcb, ent->d_name);
                t_config *fcb_config = config_create(ruta_completa);
                t_fcb *fcb = malloc(sizeof(t_fcb));
                fcb->nombre_archivo = config_get_string_value(fcb_config, "NOMBRE_ARCHIVO");
                fcb->tam_archivo = config_get_int_value(fcb_config, "TAMANIO_ARCHIVO");
                fcb->puntero_directo = config_get_int_value(fcb_config, "PUNTERO_DIRECTO");
                fcb->puntero_indirecto = config_get_int_value(fcb_config, "PUNTERO_INDIRECTO");
                list_add(lista_fcbs, fcb);
            }
        }
        closedir(dir);
    }
    return lista_fcbs;

}