#include "kernel.h"


pthread_t hiloFileSystem;
int procesosRW = 0;
void iniciar_file_system() 
{
    pthread_create(&hiloFileSystem, NULL, (void*) file_system, NULL);
    pthread_detach(hiloFileSystem);
}

void file_system()
{
    while (1)
    {
        sem_wait(&sem_bloqueados_fs);
        t_pcb *pcb; 
        pcb = sacar_pcb_cola_bloqueados_FileSystem();
        t_list* instrucciones = pcb->instrucciones;
        t_instruccion* ultima_instruccion = obtener_instruccion(pcb);
        switch (ultima_instruccion->identificador)
        {
            case F_TRUNCATE:
                solicitar_truncamiento(pcb);
                break;

            case F_READ:
                leer(ultima_instruccion, pcb);               
                set_puede_iniciar_compactacion(true);
                break;

            case F_WRITE:
                escribir(ultima_instruccion, pcb);
                set_puede_iniciar_compactacion(true);
                break;
                
            default:
                break;
        }
        agregar_a_listos(pcb);
    }
}
bool espero_compactacion = false;

void set_puede_iniciar_compactacion(bool estado)
{

    bool comp = false;
    pthread_mutex_lock(&puedeIniciarCompactacionMutex);

    if(procesosRW <= 0) comp = true;

    if(estado == false) procesosRW++;
    else procesosRW--;

    if(procesosRW <= 0 && comp == false){
        if(espero_compactacion) sem_post(&compactacion);
    }
    
    pthread_mutex_unlock(&puedeIniciarCompactacionMutex);
    

    
    
}

bool get_puede_iniciar_compactacion()
{
    bool retorno = false;
    pthread_mutex_lock(&puedeIniciarCompactacionMutex);
    if(procesosRW < 1) retorno = true;
    pthread_mutex_unlock(&puedeIniciarCompactacionMutex);
    return retorno;
}

void esperar_fin_FS(){
    if(!get_puede_iniciar_compactacion()){
        espero_compactacion = true;
        sem_wait(&compactacion);
    } 
}