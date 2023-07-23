#include "memoria.h"


void atender_cpu(void* conexion, t_log* logger)
{
	int conexion_cpu = (int)conexion;
	log_info(logger, "escuchando a cpu");
	t_pcb* pcb = recibir_pcb(conexion_cpu);
	log_info(logger, "recibi pcb");

	// hacer lo que tengo que hacer
	destruir_pcb(pcb); 
	close(conexion_cpu);

}
void atender_kernel(void* conexion, t_log* logger)
{
	int conexion_kernel = (int)conexion;
	log_info(logger, "escuchando a kernel");
	t_pcb* pcb = recibir_pcb(conexion_kernel);
	log_info(logger, "recibi pcb");
   
   

	destruir_pcb(pcb);
	close(conexion_kernel);

}

void atender_fs(void* conexion, t_log* logger)
{
	int conexion_fs = (int)conexion;
	log_info(logger, "escuchando al filesystem");
	//recibo algo
	log_info(logger, "recibi algo");

	// uso ese algo y lo destruyo 
	close(conexion_fs);

}