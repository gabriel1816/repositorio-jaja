#include "utils-shared.h"

int mi_funcion_compartida(){
    return 100;
}

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int gai = getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family,
								server_info->ai_socktype,
								server_info->ai_protocol);


	// Ahora que tenemos el socket, vamos a conectarlo

	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
	freeaddrinfo(server_info);

	return socket_cliente;
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

t_paquete* crear_super_paquete(void)
{
	//me falta un malloc!
	t_paquete* paquete;

	//descomentar despues de arreglar
	//paquete->codigo_operacion = PAQUETE;
	//crear_buffer(paquete);
	return paquete;
}

t_paquete* crear_paquete(t_buffer* buffer, int codigo_op)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo_op;
	paquete->buffer = buffer;
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	void* a_enviar = malloc(paquete->buffer->size + sizeof(int) *2);
	int offset = 0;
	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar +offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);
	memcpy(a_enviar +offset, &(paquete->buffer->stream), paquete->buffer->size);
	send(socket_cliente, a_enviar, paquete->buffer->size + sizeof(int) *2 , 0);
	free(a_enviar);
	return;
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void destruir_buffer(t_buffer* buffer)
{
	free(buffer);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}


