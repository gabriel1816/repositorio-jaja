#include "memoria.h"
/*
void compactar(t_list* tabla_segmentos){
    //recorro la tabla de segmentos
    for(int i = 0; i < list_size(tabla_segmentos); i++){
        t_segmento *segmento = list_get(tabla_segmentos, i);

        //me fijo si ese segmento pertenece a la lista de huecos
        if(segmento_es_hueco(segmento, lista_huecos)){
            list_remove(tabla_segmentos, i); //lo remuevo y lo vuelvo a poner al final de la lista
            list_add(tabla_segmentos, segmento);  
            modificar_valores_segmento(segmento, tabla_segmentos); //hay que cambiar el valor de la base y limite de todos      
        }

    }
    //ahora tengo a todos los huecos al final de la tabla
    //obtengo la base del primero y en el limite lepongo el limite del ultimo y borro todos los otros
    unir_todos_los_huecos(tabla_segmentos);

}

bool segmento_es_hueco(t_segmento *segmento, t_list* lista_huecos){
    bool es_hueco = false;
    for(int i = 0; i < list_size(lista_huecos); i++){
        t_segmento *segmento_i = list_get(lista_huecos, i);

        if(segmento == segmento_i){
            es_hueco = true;       
         }
    }

    return es_hueco;
}

//void modificar_valores_segmento(t_segmento *segmento, t_list* tabla_seg){
//     for(int i = 0; i < list_size(lista_huecos); i++){
//        t_segmento *seg1 = list_get(tabla_seg, i);
//        t_segmento *seg2 = list_get(tabla_seg, i+1);
//
//        if(i = 1){
//            seg1->base = 0;
//            seg1->limite = seg1->base + seg1->limite;
//        }
//        else{
//            seg2->base = seg1->limite;
//            seg2->limite = seg2->base + seg2->li;
//        }
//     }
//}

void unir_todos_los_huecos(t_list *tabla_segmentos){
    //encuentro al primer hueco
    t_segmento *hueco1 = primer_hueco(tabla_segmentos);

    int pos_primer_hueco;
    int ultima_posicion = list_size(tabla_segmentos);

    for(int i = 0; i < list_size(tabla_segmentos); i++){
         t_segmento *segmento = list_get(tabla_segmentos, i);

        if(segmento == hueco1){
            pos_primer_hueco = i;       
         }

         if(i > pos_primer_hueco){
            list_remove(tabla_segmentos, i); //voy removiendo todos los huecos para que solo me quede 1
         }
    }

    hueco1->limite = ultima_posicion;
  
}

t_segmento* primer_hueco(t_list *tabla_segmentos){
     t_segmento *hueco;
    int i = 0;
    bool es_hueco = false;
    while(es_hueco == false){
        t_segmento *segmento = list_get(tabla_segmentos, i);
        if(segmento_es_hueco(segmento, lista_huecos)){
            es_hueco = true;
            hueco = segmento;
        }
    }

    return hueco;
}
*/