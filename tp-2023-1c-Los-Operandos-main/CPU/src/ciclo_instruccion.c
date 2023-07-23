#include "CPU.h"


void comenzar_ciclo_de_instruccion(t_pcb* pcb, int conexion_con_kernel)
{
	log_info(logger, "comenzando ciclo de instruccion");
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
    	case MOV_OUT:
			//pido a memoria
			log_info(logger, "PID: %d - ejecutando %d - %s %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->identificador, 
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1],
					instruccion_a_ejecutar->parametros[2]);
		break;

		case F_READ: 
		case F_WRITE: 
			log_info(logger, "PID: %d - ejecutando %d - %s %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->identificador, 
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1],
					instruccion_a_ejecutar->parametros[2]);

			enviar_pcb(pcb, conexion_con_kernel, logger);
		break;

		case F_TRUNCATE:
		case F_SEEK:
		case CREATE_SEGMENT:
			log_info(logger, "PID: %d - ejecutando %d - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->identificador, 
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);

			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;

		case SIGNAL: 
		case WAIT: 
		case I_O:
		case F_OPEN: 
		case F_CLOSE:
		case DELETE_SEGMENT:
			log_info(logger, "PID: %d - ejecutando %d - %s", 
					pcb->pid ,
					instruccion_a_ejecutar->identificador, 
					instruccion_a_ejecutar->parametros[0]);

			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
		break;

		case SET: 

			log_info(logger, "PID: %d - ejecutando %d - %s %s", 
					pcb->pid ,
					instruccion_a_ejecutar->identificador, 
					instruccion_a_ejecutar->parametros[0],
					instruccion_a_ejecutar->parametros[1]);

	    	usleep(retardo_instruccion * 1000); 
			strcpy(instruccion_a_ejecutar->parametros[0], instruccion_a_ejecutar->parametros[1]);
			guardarRegistro(instruccion_a_ejecutar->parametros[0], instruccion_a_ejecutar->parametros[1], pcb);
			
		break;
		
		case YIELD:
		case EXIT:
			log_info(logger, "PID: %d - ejecutando %d", 
					pcb->pid ,
					instruccion_a_ejecutar->identificador);
			
			temporal_stop(temporizador);
			pcb->tiempo_ejecucion = temporal_gettime(temporizador);
			temporal_destroy(temporizador);
			enviar_pcb(pcb, conexion_con_kernel, logger);
			return;
    	break;
		
		default:
		break;
	}
    return;
}
