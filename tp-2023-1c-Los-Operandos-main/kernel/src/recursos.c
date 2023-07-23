#include "kernel.h"


t_dictionary* diccionario_recursos;
t_list* cola_recursos_bloqueados;

void iniciar_recursos()
{
    diccionario_recursos = dictionary_create();
    log_info(logger, "diccionario");
    for(int i=0; i < string_array_size(kernel_config.recursos); i++){

        t_recurso* recurso = malloc(sizeof(t_recurso));
        recurso->cola_bloqueados = list_create();
        recurso->instancias = kernel_config.instancias_recursos[i];
        dictionary_put(diccionario_recursos, kernel_config.recursos[i], recurso);

    }
    log_info(logger, "fors");
}

void ejecutar_signal(t_pcb* pcb, t_instruccion* instruccion)
{
    if(! dictionary_has_key(diccionario_recursos, instruccion->parametros[0])){
        terminar_proceso(pcb, "No existe el recurso solicitado por signal");
        return;
    }

    t_recurso* recurso = dictionary_get(diccionario_recursos, instruccion->parametros[0]);
    recurso->instancias ++;

    if(recurso->instancias <= 0){
        t_pcb* pcb = list_remove(recurso->cola_bloqueados, 0);
        // armo una cola especifica para que se desbloquee solo con SIGNAL
        cola_recursos_bloqueados_sacar(pcb);
        agregar_a_listos(pcb);
    }
    ejecutar(pcb);

}

void ejecutar_wait(t_pcb* pcb, t_instruccion* instruccion)
{
    if(! dictionary_has_key(diccionario_recursos, instruccion->parametros[0])){
        terminar_proceso(pcb, "No existe el recurso solicitado por wait");
        return;
    }

    t_recurso* recurso = dictionary_get(diccionario_recursos, instruccion->parametros[0]);
    recurso->instancias --;

    if(recurso->instancias <= 0){
        list_add(recurso->cola_bloqueados, pcb);
        cola_recursos_bloqueados_agregar(pcb);
        return; //se bloqueo
    }
    ejecutar(pcb);
}

void cola_recursos_bloqueados_sacar(t_pcb* pcb)
{
    for(int i=0; i < list_size(cola_recursos_bloqueados); i++){
        if(list_get(cola_recursos_bloqueados, i) == pcb){
            list_remove(cola_recursos_bloqueados, i);
            return;
        }
    }
}

void cola_recursos_bloqueados_agregar(t_pcb* pcb)
{
    list_add(cola_recursos_bloqueados, pcb);
    pcb->estado = BLOQUEADO;
    // fijarnos si hace falta mutex (no creo)
}