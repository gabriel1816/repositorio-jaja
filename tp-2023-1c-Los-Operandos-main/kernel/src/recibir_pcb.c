

#include "kernel.h"

pthread_t hilo_IO;
t_list* cola_bloqueados_IO;

void recibir_pcb_de_cpu(int cpu_socket)
{
    t_pcb* pcb_actualizado = recibir_pcb(cpu_socket, logger);
    t_instruccion* ultima_instruccion = list_get(pcb_actualizado->instrucciones, pcb_actualizado->p_counter - 1);
    t_pcb* pcb_en_ejecucion;
    if(pcb_actualizado->estado == SEG_FAULT){
        terminar_proceso(pcb_actualizado, "SEGMENTATION FAULT");
    }

    if(kernel_config.schedulerAlgorithm, "HRRN"){
        actualizar_estimado_rafaga(pcb_actualizado);
    }
    
    switch(ultima_instruccion->identificador){
        case EXIT:
            pcb_en_ejecucion = sacar_pcb_de_ejecucion();
            terminar_proceso(pcb_actualizado, "SUCCESS");
            destruir_pcb(pcb_en_ejecucion);
        break;
        case YIELD:
            pcb_en_ejecucion = sacar_pcb_de_ejecucion();
            destruir_pcb(pcb_en_ejecucion);
            agregar_a_listos(pcb_actualizado);
        break;
        case I_O:
            log_info(logger,"PID: %d - Bloqueado por: %s", ultima_instruccion->parametros[0]);
            pcb_en_ejecucion = sacar_pcb_de_ejecucion();
            destruir_pcb(pcb_en_ejecucion);
            ejecutar_IO(pcb_actualizado, ultima_instruccion);
        break;
        case SIGNAL:
            pcb_en_ejecucion = sacar_pcb_de_ejecucion();
            destruir_pcb(pcb_en_ejecucion);
            ejecutar_signal(pcb_actualizado, ultima_instruccion);
            //log_info(logger,"PID: %d - Signal: %s - Instancias: %d", ultima_instruccion->parametros[0], );
            // esto se hace despues ^^^^^^
        break;
        case WAIT:
            pcb_en_ejecucion = sacar_pcb_de_ejecucion();
            destruir_pcb(pcb_en_ejecucion);
            ejecutar_wait(pcb_actualizado, ultima_instruccion);
        break;
    }
}

t_pcb* sacar_pcb_de_ejecucion()
{
    pthread_mutex_lock(&mutex_pcb_a_ejecutar);
    t_pcb* pcb = pcb_a_ejecutar;
    pcb_a_ejecutar = NULL;
    pthread_mutex_unlock(&mutex_pcb_a_ejecutar);
    return pcb;
}

void ejecutar_IO(t_pcb* pcb, t_instruccion* instruccion)
{
    agregar_bloqueados_IO(pcb);
    pthread_create(&hilo_IO, NULL, (void*)pasar_a_IO, (void*)atoi(instruccion->parametros[0]));
    pthread_detach(hilo_IO);

}

void pasar_a_IO(int tiempo)
{
    sleep(tiempo);
    t_pcb* pcb = sacar_de_bloqueado_IO();
    agregar_a_listos(pcb);
}

void agregar_bloqueados_IO(t_pcb* pcb)
{
    list_add(cola_bloqueados_IO, pcb);
    pcb->estado = BLOQUEADO;
}

t_pcb* sacar_de_bloqueado_IO()
{
    return list_remove(cola_bloqueados_IO, 0);
}