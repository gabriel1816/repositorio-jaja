#include "CPU.h"

t_direc_fisica traducir_direccion(int direc_logica)
{
    t_direc_fisica direc_fisica;
    direc_fisica.desplazamiento = direc_logica % tam_max_segmento;
	direc_fisica.numero_segmento = floor(direc_logica / tam_max_segmento);
    return direc_fisica;
}