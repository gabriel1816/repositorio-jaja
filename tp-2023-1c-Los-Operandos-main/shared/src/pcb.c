#include "utils-shared.h"

int size_registros = sizeof(char[4]) * 4 + sizeof(char[8])* 4 + sizeof(char[16]) * 4; 

void enviar_pcb(t_pcb* pcb, int socket, t_log* logger){
	t_buffer* buffer = crear_buffer_pcb(pcb, logger);
	t_paquete* paquete = crear_paquete();
	paquete->codigo_operacion = PCB;
	agregar_a_paquete(paquete,buffer,buffer->size); 
	enviar_paquete(paquete, socket);
	eliminar_paquete(paquete);
}

t_pcb* crear_pcb(t_list* instrucciones, int conexion){
    // agregar las cosas al pcb 
    // inicializar contexto, etc....
    t_pcb* pcb = malloc(sizeof(t_pcb));
	pcb->conexion = conexion;
	pcb->pid = getpid();
	pcb->p_counter = 0;
	pcb->estado = NUEVO;
    pcb->instrucciones = instrucciones;
	pcb->tiempo_llegada = 0;
	pcb->tiempo_ejecucion = 0;
	pcb->registros = inicializar_registro();
    return pcb;
}

t_registro inicializar_registro(){
	t_registro registro;
	strcpy(registro.AX, "\0");
	strcpy(registro.BX, "\0");
	strcpy(registro.CX, "\0");
	strcpy(registro.DX, "\0");
	strcpy(registro.EAX, "\0");
	strcpy(registro.EBX, "\0");
	strcpy(registro.ECX, "\0");
	strcpy(registro.EDX, "\0");
	strcpy(registro.RAX, "\0");
	strcpy(registro.RBX, "\0");
	strcpy(registro.RCX, "\0");
	strcpy(registro.RDX, "\0");
	return registro;
}


t_buffer* crear_buffer_pcb(t_pcb* pcb, t_log* logger){
	t_buffer* buffer = crear_buffer();
	t_buffer* buffer_instrucciones = crear_buffer__para_t_lista_instrucciones(pcb->instrucciones);

	int size = sizeof(int) * 2 + //conexion y pc
				sizeof(pid_t) +
				sizeof(int64_t) +
				sizeof(double) +
				sizeof(t_estado) + //estado
				buffer_instrucciones->size + //instrucciones
                sizeof(t_registro); // registros
	buffer->size = size;

	void* stream = malloc(buffer->size);
	int offset = 0;

	//paso pcb
	memcpy(stream + offset, &pcb->conexion, sizeof(int));
	offset += sizeof(int);
	memcpy(stream + offset, &pcb->pid, sizeof(pid_t));
	offset += sizeof(pid_t);
	memcpy(stream + offset, &pcb->p_counter, sizeof(int));
	offset += sizeof(int);
	memcpy(stream + offset, &pcb->estado, sizeof(t_estado));
	offset += sizeof(t_estado);
	memcpy(stream + offset, buffer_instrucciones->stream, buffer_instrucciones->size);
    offset += sizeof(buffer_instrucciones->size);
    memcpy(stream + offset, &pcb->tiempo_ejecucion, sizeof(int));
	offset += sizeof(int64_t);
	memcpy(stream + offset, &pcb->estimado_rafaga, size_registros);
    offset += sizeof(double);
	memcpy(stream + offset, &pcb->registros, size_registros);
    
	buffer->stream = stream;
	destruir_buffer(buffer_instrucciones);
	return buffer;
}

t_pcb* recibir_pcb(int socket_cliente)
{
	t_paquete* paquete = recibir_paquete(socket_cliente);
	t_pcb* pcb = deserializar_buffer_pcb(paquete->buffer);

	eliminar_paquete(paquete);
	return pcb;
}

t_pcb* deserializar_buffer_pcb(t_buffer* buffer)
{
	t_pcb* pcb = malloc(sizeof(t_pcb));

	void* stream = buffer->stream;
	int size_restante = buffer->size;
	
	memcpy(&(pcb->conexion), stream, sizeof(int));
	stream += sizeof(int);
	size_restante -= sizeof(int);
	memcpy(&(pcb->pid), stream, sizeof(int));
	stream += sizeof(pid_t);
	size_restante -= sizeof(int);
	memcpy(&(pcb->p_counter), stream, sizeof(int));
	stream += sizeof(int);
	size_restante -= sizeof(int);
	memcpy(&(pcb->estado), stream, sizeof(t_estado));
    size_restante -= sizeof(t_estado);
    memcpy(&(pcb->tiempo_ejecucion), stream, sizeof(int));
	stream += sizeof(int64_t);
    memcpy(&(pcb->tiempo_llegada), stream, sizeof(int));
	stream += sizeof(int);
	memcpy(&(pcb->estimado_rafaga), stream, sizeof(double));
	stream += sizeof(double);
	memcpy(&(pcb->registros), stream, size_registros);

	buffer->size = size_restante;
	pcb->instrucciones = list_create();
	t_list* lista_instrucciones = deserializar_lista_instrucciones(buffer);
	stream += sizeof(lista_instrucciones);
	size_restante -= sizeof(lista_instrucciones);

	return pcb; 
	
}

void destruir_pcb(t_pcb* pcb)
{
	list_destroy_and_destroy_elements(pcb->instrucciones, (void*)destruir_instruccion);
	free(pcb);
}