#include "kernel.h"


void atender_consola(void* conexion)
{
	int conexion_consola = (int)conexion;
	log_info(logger, "escuchando a consola");
    t_paquete* paquete_instrucciones = recibir_paquete(conexion_consola, logger);
	log_info(logger, "paquete recibido");
	t_list* lista_instrucciones = crear_lista_instrucciones_para_el_buffer(paquete_instrucciones->buffer);
	log_info(logger, "recibi lista de instrucciones");
	t_pcb* pcb = crear_pcb(lista_instrucciones, conexion_consola);
	log_info(logger, "Se crea el proceso %d en NEW", pcb->pid);
	agregar_a_nuevos(pcb);
		

	//list_destroy_and_destroy_elements(lista_instrucciones, destruir_instruccion);
	//close(conexion_consola);
}

void atender_cpu(void* conexion)
{
	int conexion_cpu = (int)conexion;
	log_info(logger, "escuchando a cpu");
	t_pcb* pcb = recibir_pcb(conexion_cpu, logger);
	log_info(logger, "recibi pcb");

	// hacer lo que tengo que hacer
	destruir_pcb(pcb); 
	close(conexion_cpu);

}
void atender_memoria(void* conexion)
{
	int conexion_memoria = (int)conexion;
	log_info(logger, "escuchando a memoria");
	//recibo algo
	log_info(logger, "recibi algo");

	// uso ese algo y lo destruyo 
	close(conexion_memoria);

}

void atender_fs(void* conexion)
{
	int conexion_fs = (int)conexion;
	log_info(logger, "escuchando al filesystem");
	//recibo algo
	log_info(logger, "recibi algo");

	// uso ese algo y lo destruyo 
	close(conexion_fs);

}