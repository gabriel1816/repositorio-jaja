#include "memoria.h"

void escuchar(int puerto_escucha) {

    pthread_t thread_cpu;
    pthread_t thread_filesystem;
    pthread_t thread_kernel;

    while (1) {
        int cliente = esperar_cliente(puerto_escucha);
        char tipo_cliente[25];
        recv(cliente, tipo_cliente, sizeof(tipo_cliente), 0);
        
        if (strcmp(tipo_cliente, "hola, soy kernel") == 0) {
            pthread_create(&thread_kernel, NULL, atender_kernel, (void*)cliente);

        } else if (strcmp(tipo_cliente, "hola, soy filesystem") == 0) {
            pthread_create(&thread_filesystem, NULL, atender_fs, (void*)cliente);

        } else if (strcmp(tipo_cliente, "hola, soy cpu") == 0) {
            pthread_create(&thread_cpu, NULL, atender_cpu, (void*)cliente);

        } else {
            log_error(logger, "Cliente desconocido");
            break;
        }
    }

    pthread_join(thread_cpu, NULL);
    pthread_join(thread_filesystem, NULL);
    pthread_join(thread_kernel, NULL);

    return NULL;
}


void atender_cpu(void* conexion)
{
	int conexion_cpu = (int) conexion;
	log_info(logger, "Se conecto la cpu a la memoria");
    bool continuar = true;
    t_paquete* paquete;
    while (continuar)
    {
        paquete = recibir_paquete(conexion_cpu, logger);
        uint32_t offset = 0;
        t_instruccion* pedidoMemoria = crear_instruccion_para_el_buffer(paquete->buffer, &offset);
       
        pid_t pid = (pid_t)strtoul(pedidoMemoria->parametros[0], NULL, 10);
        int32_t direccion_fisica = (int32_t)strtoul(pedidoMemoria->parametros[1], NULL, 10);
        char* valor_registro = malloc(sizeof(char)*17);
        t_paquete* respuesta;
        uint32_t tamanio_registro;
        t_buffer* buffer;
        t_list* parametros_respuesta = list_create();
        t_instruccion* respuesta_memoria;
        switch (pedidoMemoria->identificador) {
            case MOV_IN:
                tamanio_registro = (uint32_t)strtoul(pedidoMemoria->parametros[2], NULL, 10); 
                void* inicio_lectura = memoria_fisica + direccion_fisica;
                memcpy(valor_registro, inicio_lectura, sizeof(char)*tamanio_registro);
               
                list_add(parametros_respuesta, valor_registro);
                respuesta_memoria = crear_instruccion(MOV_IN, parametros_respuesta);
                buffer = crear_buffer_para_t_instruccion(respuesta_memoria);
                respuesta = crear_paquete(buffer, CODIGO_INSTRUCCION_MEMORIA);
                log_info(logger, "PID: %u - Acción: LEER - Dirección física: %d - Tamaño: %u - Origen: CPU", pid, direccion_fisica, tamanio_registro);
            break;

            case MOV_OUT:
                valor_registro = pedidoMemoria->parametros[2];
                tamanio_registro = sizeof(char)*strlen(valor_registro);
                memcpy((memoria_fisica+direccion_fisica), valor_registro, tamanio_registro);
                respuesta_memoria = crear_instruccion(MOV_OUT, parametros_respuesta);
                buffer = crear_buffer_para_t_instruccion(parametros_respuesta);
                respuesta = crear_paquete(buffer, MOV_OUT);
                log_info(logger, "PID: %u - Acción: ESCRIBIR - Dirección física: %d - Tamaño: %u - Origen: CPU", pid, direccion_fisica, tamanio_registro);
                valor_registro = "";
            break;

            default:
                log_info(logger, "Mensaje no reconocido");
            break;
        }
        usleep(retardo_memoria*1000);
        enviar_paquete(conexion_cpu, respuesta, logger);
   		destruir_paquete(respuesta);
        destruir_paquete(paquete);
        destruir_instruccion(respuesta_memoria);
        //free(valor_registro);
    }
    log_info(logger, "Se cerrará la conexión (CPU - Memoria)");
    log_info(logger, "Se desconectó un cliente");
    close(conexion_cpu);

}

void atender_kernel(void* conexion)
{
	int conexion_kernel = (int)conexion;
	log_info(logger, "escuchando a kernel");

	while(1){
		t_paquete *paquete = recibir_paquete(conexion_kernel, logger); 
		uint32_t offset = 0;
		t_instruccion *instruccion =  crear_instruccion_para_el_buffer(paquete->buffer, &offset); 
		pid_t pid;
		switch(instruccion->identificador){
			case CREATE_SEGMENT:
			t_segmento *nuevo_segmento;
			t_list *huecos_libres;
			int id_segmento = instruccion->parametros[0];
			int tamanio_seg = instruccion->parametros[1];
			int cant_huecos_nec = tamanio_seg / (tamanio_segmento_0);
			if(hay_espacio(lista_huecos)){
				//me fijo si hay la cant de hucos disponibles continuos que necesito 
			if(hay_huecos_continuos(cant_huecos_nec, algoritmo, tamanio_seg, huecos_libres)){
				for(int i = 0; i < cant_huecos_nec; i++){
					t_segmento *hueco1 = list_get(huecos_libres, i);
					nuevo_segmento = crear_segmento(hueco1->base, hueco1->limite, id_segmento, 0, pid);
				}
			}
			else{
				//le mando a kernel que tengo que compactar
				//kernel me manda que tengo que comparespuestar
				//compactar(tablas_segmentos);
				//le aviso a kernel que ya compacte y le mando como quda la nueva tabla de segmentos
				//kernel me manda de nuevo la instruccion y ahora si voy a poder crear el segmento
			}
		
			break;
			case DELETE_SEGMENT:
				borrarSegmento(instruccion->parametros[0]);
			break;
			case CREAR_PROCESO:
				pid = (pid_t)atoi(instruccion->parametros[0]);
				crear_proceso_memoria(pid, conexion);
			break;
			case ELIMINAR_PROCESO:
				//eliminar_proceso(pid);
			break;
		}

	}

	close(conexion_kernel);
	}
}

void atender_fs(void* conexion)
{
	int conexion_fs = (int)conexion;
	log_info(logger, "escuchando al filesystem");
	t_pcb* pcb = recibir_pcb(conexion_fs, logger);
	//recibo algo
	t_paquete *paquete = recibir_paquete(conexion_fs, logger); 
	t_instruccion *instruccion =  crear_instruccion_para_el_buffer(paquete->buffer, 0); //la cpu le manda las instrucciones que tienen que ver con memoria
	log_info(logger, "recibi algo");

	// uso ese algo y lo destruyo 
	close(conexion_fs);

}




bool hay_espacio(t_list *segmentos_vacios)
{
	int espacio_total;
	for(int i = 0; i < list_size(segmentos_vacios); i++){
		t_segmento *segmento = list_get(segmentos_vacios, i);
		espacio_total += obtener_tam_segmento(segmento);
	}
}

int obtener_tam_segmento(t_segmento *segmento)
{
	return segmento->limite - segmento->base;
}

bool hay_huecos_continuos(int cant_huecos_libres, t_algoritmo algoritmo_asignacion, int tamanio, t_list *huecos_disponibles){
	//me fijo si hay x cant de huecos libres e
	bool hay_huecos_disp = true;

	if(algoritmo_asignacion == FIRST_FIT){
		for(int i = 0; i < cant_huecos_libres; i++){
			list_add(huecos_disponibles, first_fit()); //me da x cant de huecos disp
		}
	}
	if(algoritmo_asignacion == BEST_FIT){
		for(int i = 0; i < cant_huecos_libres; i++){
			list_add(huecos_disponibles, best_fit(tamanio)); //me da x cant de huecos disp
		}
	}
	if(algoritmo_asignacion == WORST_FIT){
		for(int i = 0; i < cant_huecos_libres; i++){
			list_add(huecos_disponibles, worst_fit(tamanio)); //me da x cant de huecos disp
		}
	}

//ahora me fijo si son continuos esos huecos
	for(int i = 0; i < list_size(huecos_disponibles); i++){
	 t_segmento *hueco1 = list_get(huecos_disponibles, i);
	 t_segmento *hueco2 = list_get(huecos_disponibles, i+1);
	 
	 int limite_hueco1 = hueco1->limite;
	 int base_hueco2 = hueco2->limite;

	 if(base_hueco2 - limite_hueco1 == 1){
		//son continuas
	 }
	 else{
		hay_huecos_disp = false;
	 }
	
	}

	return hay_huecos_disp;
}


t_segmento* first_fit()
{ //me devuelven el numero del hueco
	//busca el primer hueco disponible
	t_segmento *hueco_libre = list_get(lista_huecos, 0);

	return hueco_libre;
}

t_segmento* best_fit(int tamanio_necesitado)
{
	//buca el hueco mas chico en el que entre el segmento
	t_segmento *hueco;
	for(int i = 0; i < list_size(lista_huecos); i++){
		
		t_segmento *hueco_libre = list_get(lista_huecos, i);
		int tam_hueco = hueco_libre->limite - hueco_libre->base;
	
		int min = 100;
		if(tamanio_necesitado <= tam_hueco && tam_hueco <= min){
			hueco = list_get(lista_huecos, i);
			min = tam_hueco;
		}
	}

	return hueco;
}

t_segmento* worst_fit(int tamanio_necesitado)
{
	//busca el hueco mas grande en el que entre 
	t_segmento *hueco;
	for(int i = 0; i < list_size(lista_huecos); i++){
		
		t_segmento *hueco_libre = list_get(lista_huecos, i);
		int tam_hueco = hueco_libre->limite - hueco_libre->base;
	
		int max = 0;
		if(tamanio_necesitado <= tam_hueco && tam_hueco >= max){
			hueco = list_get(lista_huecos, i);
			max = tam_hueco;
		}
	}
	return hueco;
}




