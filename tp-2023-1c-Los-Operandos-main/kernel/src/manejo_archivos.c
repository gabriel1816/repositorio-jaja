#include "kernel.h"

void agregar_a_tabla_global(char* NombreArchivo) 
{
    archivo_abierto* archivo = malloc(sizeof(archivo_abierto));
    archivo->nombre_archivo = malloc(sizeof(NombreArchivo));
    strcpy(archivo->nombre_archivo, NombreArchivo);
    archivo->puntero = 0;
    archivo->archivo = crear_recurso(1);
    dictionary_put(diccionario_archivos, NombreArchivo, archivo->archivo);
    list_add(tabla_global_archivos, archivo);
}


void agregar_a_tabla_proceso(t_pcb* pcb, char* Nombrearchivo) 
{
    archivo_abierto* archivo = malloc(sizeof(archivo_abierto));
    archivo->nombre_archivo = Nombrearchivo;
    archivo->puntero = 0;
    list_add(pcb->archivos_abiertos, archivo);
}


bool esta_en_tabla(char* Nombrearchivo) 
{
    for(int i = 0; i < list_size(tabla_global_archivos); i++) {
        archivo_abierto* archivo = list_get(tabla_global_archivos, i);
        if(string_equals_ignore_case(archivo->nombre_archivo, Nombrearchivo)) {
            return true;  
        }
    }
    return false;
}


void sacar_de_tabla_proceso(t_pcb* pcb, char* archivo) 
{
    eliminar_archivo_por_nombre(pcb->archivos_abiertos, archivo);
}


void sacar_de_tabla_global(t_pcb* pcb_actualizado, char* archivo) 
{
    eliminar_archivo_por_nombre(tabla_global_archivos, archivo);
}


void eliminar_archivo_por_nombre(t_list* lista, char* nombre) 
{
    for(int i = 0; i < list_size(lista); i++) {
        archivo_abierto* archivo = list_get(lista, i);

        if(strcmp(archivo->nombre_archivo, nombre) == 0) {
            archivo_abierto* archivo_eliminado = list_remove(lista, i);
            break;
        }
    }
}


void actualizar_puntero_tabla_global(char* nombre_archivo, char* puntero) 
{
    int punteroN = atoi(puntero);
    for(int i = 0; i < list_size(tabla_global_archivos); i++) {
        archivo_abierto* archivo = list_get(tabla_global_archivos, i);
        if(string_equals_ignore_case(archivo->nombre_archivo, nombre_archivo)) {
            archivo->puntero = punteroN;
            break;
        }
    }
}

void desbloquear_proceso(t_pcb* pcb, char* key_archivo) 
{
    int indice_a_remover = -1;
    t_recurso* archivo = dictionary_get(diccionario_archivos, key_archivo);
    for(int i = 0; i < list_size(archivo->cola_bloqueados); i++) {
        t_pcb* pcb_bloqueado = list_get(archivo->cola_bloqueados, i);
            if(pcb_bloqueado == pcb) {
            indice_a_remover = i;
            break;
        }
    }
    if(indice_a_remover != -1) {
        t_pcb* pcb_bloqueado = list_remove(archivo->cola_bloqueados, indice_a_remover);
        agregar_a_listos(pcb_bloqueado);
    }
}  


bool hay_procesos_en_espera(t_pcb* pcb, char* key_archivo) 
{
    t_recurso* archivo = dictionary_get(diccionario_archivos, key_archivo);
    if(archivo->instancias < 0) return true;
    return false;
}


void atender_signal_archivo(t_pcb* pcb, char* key_archivo)
{
    if(!dictionary_has_key(diccionario_archivos, key_archivo)){
        terminar_proceso(pcb, "SIGNAL DE ARCHIVO INEXISTENTE");
        return;
    }

    t_recurso* archivo = dictionary_get(diccionario_archivos, key_archivo);

    archivo->instancias++;

    log_info(logger, "PID: %d - Signal: %s - Instancias: %d", pcb->pid, key_archivo, archivo->instancias);

    if(archivo->instancias <= 0){
        t_pcb* pcb_bloqueado = list_remove(archivo->cola_bloqueados, 0);
        sacar_pcb_cola_bloqueados_RecursoArchivo(pcb_bloqueado);
        agregar_a_listos(pcb_bloqueado);
    }
}

t_recurso* crear_recurso(int instancias)
{
    t_recurso* recurso = malloc(sizeof(t_recurso));
    recurso->cola_bloqueados = list_create();
    recurso->instancias = instancias;
    return recurso;
}

void atender_wait_archivo(t_pcb* pcb, char* key_archivo)
{

    if(!dictionary_has_key(diccionario_archivos, key_archivo)){
        terminar_proceso(pcb, "WAIT DE ARCHIVO INEXISTENTE");
        return;
    }
    t_recurso* archivo = dictionary_get(diccionario_archivos, key_archivo);

    archivo->instancias--;

    if(archivo->instancias < 0){
        list_add(archivo->cola_bloqueados, pcb);
        log_info(logger, "PID: %d - Bloqueado por FOPEN: %s", pcb->pid, key_archivo);
        agregar_pcb_cola_bloqueados_RecursoArchivo(pcb);
        return;
    }
}