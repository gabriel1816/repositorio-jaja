#include "fileSystem.h"

int calcular_bloques_adicionales(uint32_t nuevo_tamanio, t_fcb *fcb)
{

    double tamanio_archivo = (double)fcb->tam_archivo;
    double nuevo_tam = (double)nuevo_tamanio;
    double tamanio_bloque = (double)superbloque->tam_bloque;

    int bloques_actuales = (int)ceil(tamanio_archivo / tamanio_bloque);
    int bloques_nuevos = (int)ceil(nuevo_tam / tamanio_bloque);

    int bloquesAdicionales = bloques_nuevos - bloques_actuales;

    return bloquesAdicionales;
}

int calcular_bloques_a_liberar(uint32_t nuevo_tamanio, t_fcb *fcb)
{

    double tamanio_archivo = (double)fcb->tam_archivo;
    double nuevo_tam = (double)nuevo_tamanio;
    double tamanio_bloque = (double)superbloque->tam_bloque;

    int bloques_actuales = (int)ceil(tamanio_archivo / tamanio_bloque);
    int bloques_nuevos = (int)ceil(nuevo_tam / tamanio_bloque);

    int bloques_a_liberar = bloques_actuales - bloques_nuevos;

    return bloques_a_liberar;
}

uint32_t buscar_bloque_libre(t_bitarray *bitmap)
{

    uint32_t bloque_libre = -1;

    uint32_t tamanio_bitmap = bitarray_get_max_bit(bitmap);
    for (int i = 0; i < tamanio_bitmap; i++)
    {

        if (!bitarray_test_bit(bitmap, i))
        { // el bit en la pos i del bitmap es 0 osea esta libre =>
            log_info(logger, "Acceso a Bitmap - Bloque: %d - Estado: %d", i, bitarray_test_bit(bitmap, i));
            bitarray_set_bit(bitmap, i); // lo seteo como ocupado (1)

            bloque_libre = i;
            break;
        }
    }
    return bloque_libre;
}

void liberar_bloques(t_fcb *fcb, int bloques_a_liberar)
{
    int cant_bloques_fcb = ceil(fcb->tam_archivo/superbloque->tam_bloque);
    int archivo_bloques = open(path_bloques, O_RDWR);

    while (bloques_a_liberar > 0 && cant_bloques_fcb > 1)
    {
        uint32_t bloque_leido;
        lseek(archivo_bloques, (int)fcb->puntero_indirecto * superbloque->tam_bloque + (cant_bloques_fcb - 1) * sizeof(uint32_t), SEEK_SET); 
        read(archivo_bloques, &bloque_leido, sizeof(uint32_t));
        sleep(retardo_acceso_bloque / 1000);
        liberar_bloque_bitmap(bloque_leido);
        cant_bloques_fcb--;
        bloques_a_liberar--;
    }
    if (cant_bloques_fcb == 1 && bloques_a_liberar == 1)
    {
        liberar_bloque_bitmap(fcb->puntero_directo);
        liberar_bloque_bitmap(fcb->puntero_indirecto);
    }
}

int liberar_bloque_bitmap(uint32_t bloque_a_liberar)
{
    log_info(logger, "Acceso a Bitmap - Bloque: %u - Estado: %d", bloque_a_liberar, bitarray_test_bit(bitmap, bloque_a_liberar));
    bitarray_clean_bit(bitmap, bloque_a_liberar);
}

void asignar_bloques(t_fcb *fcb, uint32_t bloques_adicionales)
{
    uint32_t bloques_asignados = ceil(fcb->tam_archivo/superbloque->tam_bloque);
    for (uint32_t i = 0; i < bloques_adicionales; i++)
    {
        uint32_t bloque_libre = buscar_bloque_libre(bitmap);
        if (bloque_libre != -1)
        { 
            escribir_bloque_en_archivo(fcb->puntero_indirecto, bloques_asignados - 1 + i, bloque_libre);
        }
        else
        {
            log_error(logger, "No hay bloques libres\n");
            break;
        }
    }
    bloques_asignados += bloques_adicionales;
}

void escribir_bloque_en_archivo(uint32_t bloque, uint32_t pos_en_bloque, uint32_t valor)
{
    int archivoBloques = open(path_bloques, O_RDWR); // Abre el archivo para lectura y escritura

    if (archivoBloques == -1)
    {
        log_error(logger, "No se pudo abrir el archivo de bloques");
        return;
    }

    lseek(archivoBloques, bloque * superbloque->tam_bloque + pos_en_bloque * sizeof(uint32_t), SEEK_SET); // Busca la posición correcta en el bloque
    sleep(retardo_acceso_bloque / 1000);
    write(archivoBloques, &valor, sizeof(uint32_t));

    close(archivoBloques);
}