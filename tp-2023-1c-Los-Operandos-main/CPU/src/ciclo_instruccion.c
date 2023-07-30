#include "CPU.h"


void comenzar_ciclo_de_instruccion(t_pcb* pcb, int conexion_con_kernel)
{
	log_info(logger, "comenzando ciclo de instruccion para el proceso PID: %d", pcb->pid);
    t_instruccion* instruccion = fetch(pcb);
    decode_y_execute(instruccion, pcb, conexion_con_kernel);
}

t_instruccion* fetch(t_pcb *pcb)
{
    t_instruccion *siguiente_instruccion = list_get(pcb->instrucciones, pcb->p_counter);
    pcb->p_counter++;
    return siguiente_instruccion;
}

void decode_y_execute(t_instruccion* instruccion_a_ejecutar, t_pcb* pcb, int conexion_con_kernel)
{ 
	// faltan logs de memoria
	switch(instruccion_a_ejecutar->identificador){
		t_buffer* buffer;
		t_paquete* paquete;
		case MOV_IN:
			log_info(logger, "PID: %d - ejecutando MOV_IN - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);
			pcb->direccion_fisica = traducir_direccion(atoi(instruccion_a_ejecutar->parametros[1]), pcb, conexion_con_kernel, tamanio_registro(instruccion_a_ejecutar->parametros[0]));
			enviar_instrucciones(instruccion_a_ejecutar, conexion_con_memoria, logger);
		break;    	
		case MOV_OUT:
			log_info(logger, "PID: %d - ejecutando MOV_OUT - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);

			pcb->direccion_fisica = traducir_direccion(atoi(instruccion_a_ejecutar->parametros[0]), pcb, conexion_con_kernel, tamanio_registro(instruccion_a_ejecutar->parametros[1]));
			instruccion_a_ejecutar->parametros[2] = tamanio_registro(instruccion_a_ejecutar->parametros[1]);
			instruccion_a_ejecutar->cant_parametros++;
			enviar_instrucciones(instruccion_a_ejecutar, conexion_con_memoria, logger);
		break;
		case F_READ: 
			log_info(logger, "PID: %d - ejecutando F_READ -%d - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1],
					instruccion_a_ejecutar->parametros[2]);

			enviar_pcb(pcb, conexion_con_kernel, logger);
		break;
		case F_WRITE: 
			log_info(logger, "PID: %d - ejecutando F_WRITE - %s %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1],
					instruccion_a_ejecutar->parametros[2]);

			enviar_pcb(pcb, conexion_con_kernel, logger);
		break;
		case F_TRUNCATE:
			log_info(logger, "PID: %d - ejecutando F_TRUNCATE - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);

			enviar_pcb(pcb, conexion_con_kernel, logger);
		break;
		case F_SEEK:
			log_info(logger, "PID: %d - ejecutando F_SEEK - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);

			enviar_pcb(pcb, conexion_con_kernel, logger);
		break;
		case CREATE_SEGMENT:
			log_info(logger, "PID: %d - ejecutando CREATE_SEGMENT - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);

			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;
		case SIGNAL: 
			log_info(logger, "PID: %d - ejecutando SIGNAL - %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0]);

			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;
		case WAIT: 
			log_info(logger, "PID: %d - ejecutando WAIT - %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0]);

			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			temporal_destroy(temporizador);
			return;
		break;
		case I_O:
			log_info(logger, "PID: %d - ejecutando I/O - %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0]);

			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;
		case F_OPEN: 
			log_info(logger, "PID: %d - ejecutando F_OPEN - %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0]);

			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;
		case F_CLOSE:
			log_info(logger, "PID: %d - ejecutando F_CLOSE - %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0]);

			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;
		case DELETE_SEGMENT:
			log_info(logger, "PID: %d - ejecutando DELETE_SEGMENT - %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0]);

			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;
		case SET: 
			log_info(logger, "PID: %d - ejecutando SET - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);

	    	usleep(retardo_instruccion * 1000);
			guardarRegistro(instruccion_a_ejecutar->parametros[0], instruccion_a_ejecutar->parametros[1], &pcb->registros);
			
		break;
		case YIELD:
			log_info(logger, "PID: %d - ejecutando YIELD", 
					pcb->pid);
			
			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;
		case EXIT:
			log_info(logger, "PID: %d - ejecutando EXIT", 
					pcb->pid);
			
			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			mostrar_por_pantalla(pcb->registros);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
    	break;
		default:
		break;
	}
    log_info(logger, "Actualización del Program Counter");
	comenzar_ciclo_de_instruccion(pcb, conexion_con_kernel);
}
