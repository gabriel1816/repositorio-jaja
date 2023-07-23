#include "kernel.h"

pthread_mutex_t mutex_cola_listos;
pthread_mutex_t mutex_cola_bloqueados;
pthread_mutex_t mutex_cola_nuevos;

void agregar_a_listos(t_pcb* pcb)
{
    pthread_mutex_lock(&mutex_cola_listos);
    list_add(cola_ready, pcb);
    pcb->tiempo_llegada = temporal_create();
    pthread_mutex_unlock(&mutex_cola_listos);
    sem_post(&sem_ready);
    // log_info(logger, "Cola Ready %d:  %???", kernel_config.schedulerAlgorithm, ???)
}

t_pcb* sacar_de_listos()
{
    t_pcb* pcb;
    pthread_mutex_lock(&mutex_cola_listos);
    pcb = list_remove(cola_ready, 0);
    temporal_stop(pcb->tiempo_llegada);
    pthread_mutex_unlock(&mutex_cola_listos);
    return pcb;
}

void agregar_a_bloqueados(t_pcb* pcb)
{
    pthread_mutex_lock(&mutex_cola_bloqueados);
    list_add(cola_blocked, pcb);
    pthread_mutex_unlock(&mutex_cola_bloqueados);
}

t_pcb* sacar_de_bloqueados()
{
    t_pcb* pcb;
    pthread_mutex_lock(&mutex_cola_bloqueados);
    pcb = list_remove(cola_blocked, 0);
    pthread_mutex_unlock(&mutex_cola_bloqueados);
    return pcb;
} 
void agregar_a_nuevos(t_pcb* pcb)
{
    pthread_mutex_lock(&mutex_cola_nuevos);
    list_add(cola_new, pcb);
    pthread_mutex_unlock(&mutex_cola_nuevos);
    sem_post(&sem_nuevos);
}

t_pcb* sacar_de_nuevos()
{
    t_pcb* pcb;
    pthread_mutex_lock(&mutex_cola_nuevos);
    pcb = list_remove(cola_new, 0);
    pthread_mutex_unlock(&mutex_cola_nuevos);
    return pcb;
} 