#include "utils-shared.h"

void enviar_instrucciones(int socket, t_list *lista_instrucciones, t_log *logger)
{
    log_info(logger, "enviando");
    t_buffer *buffer = crear_buffer__para_t_lista_instrucciones(lista_instrucciones);
    t_paquete *paquete = crear_paquete(buffer, 20);
    enviar_paquete(paquete, socket);
    log_info(logger, "instrucciones enviadas");
    eliminar_paquete(paquete);
}



t_buffer *crear_buffer__para_t_lista_instrucciones(t_list *lista_instrucciones) // creo un buffer para muchas instrucciones
{
    t_buffer *buffer = malloc(sizeof(t_buffer));
    int size_total = 0;
    for (int i = 0; i < list_size(lista_instrucciones); i++)
    {
        t_instruccion *instruccion = list_get(lista_instrucciones, i);
        t_buffer *buffer_instruccion = crear_buffer_para_t_instruccion(instruccion); // 10 - 20 - 4
        size_total += buffer_instruccion->size;
        destruir_buffer(buffer_instruccion);
    }
    // creo el stream y copio los datos de cada buffer
    void *stream = malloc(size_total);
    buffer->size = size_total;
    int offset = 0;
    for (int i = 0; i < list_size(lista_instrucciones); i++)
    {
        t_buffer *buffer_instruccion = crear_buffer_para_t_instruccion(list_get(lista_instrucciones, i));
        int size = buffer_instruccion->size;
        void *stream_instruccion = buffer_instruccion->stream;
        memcpy(stream + offset, stream_instruccion, size);
        offset += size;
        destruir_buffer(buffer_instruccion);
    }
    buffer->stream = stream;
    return buffer;
}



t_buffer *crear_buffer_para_t_instruccion(t_instruccion *instruccion) // creo un buffer para una instruccion
{
    t_buffer *buffer = malloc(sizeof(t_buffer));
    buffer->size = sizeof(int) +                         // codigo
                   sizeof(int) +                         // cant_parametros
                   espacio_de_array_parametros(instruccion) + // p1 + p2 + p3 + p4
                   sizeof(int) * 4;                      // param_length1, param_length2, param_length3, param_length4
    void *stream = calloc(buffer->size, 1);
    ;
    int offset = 0;
    memcpy(stream + offset, &instruccion->identificador, sizeof(t_identificador));
    offset += sizeof(int);
    memcpy(stream + offset, &instruccion->cant_parametros, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &instruccion->param_length1, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &instruccion->param_length2, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &instruccion->param_length3, sizeof(int));
    offset += sizeof(int);
    memcpy(stream + offset, &instruccion->param_length4, sizeof(int));
    offset += sizeof(int);
    for (int i = 0; i < instruccion->cant_parametros; i++)
    {
        memcpy(stream + offset, instruccion->parametros[i], strlen(instruccion->parametros[i]) + 1);
        offset += strlen(instruccion->parametros[i]) + 1;
    }
    buffer->stream = stream;
    return buffer;
}




int espacio_de_array_parametros(t_instruccion *instruccion)
{
    int espacio = 0;
    for (int i = 0; i < instruccion->cant_parametros; i++)
        espacio += strlen(instruccion->parametros[i]) + 1;
    return espacio;
}