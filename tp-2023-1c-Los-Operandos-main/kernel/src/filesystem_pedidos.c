#include "kernel.h"

bool solicitar_apertura(t_pcb* pcb) {

    enviar_pcb(pcb, filesystem_socket, logger);
    char* respuesta[20];
    t_instruccion* ultima_instruccion = obtener_instruccion(pcb);
    recv(filesystem_socket, respuesta, sizeof(respuesta), 0);

    if(!strcmp(respuesta, "abierto")) {
        return true;
    }
    else {
        log_info(logger, "PID: %d - Crear Archivo: %s", pcb->pid, ultima_instruccion->parametros[0]);
        solicitar_creacion(ultima_instruccion); 
    }
}

void solicitar_creacion(t_instruccion* instruccion) {

    t_list* parametros = list_create();
    list_add(parametros, instruccion->parametros[0]);
    t_instruccion* instruccion_pedidofs = crear_instruccion(F_CREATE, parametros);
    t_buffer* buffer = crear_buffer_para_t_instruccion(instruccion_pedidofs);
    t_paquete* paquete = crear_paquete(buffer, CODIGO_INSTRUCCION);
    enviar_paquete(filesystem_socket, paquete, logger);
    char* respuesta[20];
    recv(filesystem_socket, respuesta, sizeof(respuesta), 0);
    destruir_paquete(paquete);

}

void solicitar_truncamiento(t_pcb* pcb) {
    enviar_pcb(pcb, filesystem_socket, logger);
    char* respuesta[20];
    recv(filesystem_socket, respuesta, sizeof(respuesta), 0);

}

void leer(t_instruccion* instruccion, t_pcb* pcb) {

    archivo_abierto* archivo = buscar_archivo(instruccion->parametros[0]);
    t_list* parametros = list_create();
    list_add(parametros, instruccion->parametros[0]);
    list_add(parametros, string_itoa(archivo->puntero));
    list_add(parametros, instruccion->parametros[2]);
    t_instruccion* pedidoFS = crear_instruccion(F_READ, parametros);
    list_clean(parametros);
    list_add(parametros, pedidoFS);
    log_info("PID: %d - Leer Archivo: %s - Puntero %d - Dirección Memoria %d - Tamaño %s",pcb->pid, instruccion->parametros[0],archivo->puntero, pcb->direccion_fisica, instruccion->parametros[2]);

    t_pcb* pcbcito = crear_pcb(pcb->conexion, parametros, EJECUTANDO, pcb->estimado_rafaga);
    pcbcito->p_counter++;
    pcbcito->direccion_fisica = pcb->direccion_fisica;
    enviar_pcb(pcbcito, filesystem_socket, logger);
    char* respuesta[20];
    recv(filesystem_socket, respuesta, sizeof(respuesta), 0);

}

void escribir(t_instruccion* instruccion, t_pcb* pcb) {

    archivo_abierto* archivo = buscar_archivo(instruccion->parametros[0]);
    t_list* parametros = list_create();
    list_add(parametros, instruccion->parametros[0]);
    list_add(parametros, string_itoa(archivo->puntero));
    list_add(parametros, instruccion->parametros[2]);
    t_instruccion* pedidoFS = crear_instruccion(F_WRITE, parametros);
    list_clean(parametros);
    list_add(parametros, pedidoFS);
    log_info("PID: %d - Escribir Archivo: %s - Puntero %d - Dirección Memoria %d - Tamaño %s",pcb->pid, instruccion->parametros[0],archivo->puntero, pcb->direccion_fisica, instruccion->parametros[2]);
    t_pcb* pcbcito = crear_pcb(pcb->conexion, parametros, EJECUTANDO, pcb->estimado_rafaga);
    pcbcito->p_counter++;
    pcbcito->direccion_fisica = pcb->direccion_fisica;
    enviar_pcb(pcbcito, filesystem_socket, logger);
    char* respuesta[20];
    recv(filesystem_socket, respuesta, sizeof(respuesta), 0);
}