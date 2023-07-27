#include "CPU.h"


void guardarRegistro(char* registro, char* nuevo_valor, t_registro* registros){

	if (string_equals_ignore_case(registro, "AX"))
	{
		strncpy(registros->AX, nuevo_valor, 4);
		registros->AX[sizeof(registros->AX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "BX"))
	{
		strncpy(registros->BX, nuevo_valor, 4);
		registros->BX[sizeof(registros->BX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "CX"))
	{
		strncpy(registros->CX, nuevo_valor, 4);
		registros->CX[sizeof(registros->CX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "DX"))
	{
		strncpy(registros->DX, nuevo_valor, 4);
		registros->DX[sizeof(registros->DX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "EAX"))
	{
		strncpy(registros->EAX, nuevo_valor, 8);
		registros->EAX[sizeof(registros->EAX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "EBX"))
	{
		strncpy(registros->EBX, nuevo_valor, 8);
		registros->EBX[sizeof(registros->EBX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "ECX"))
	{
		strncpy(registros->ECX, nuevo_valor, 8);
		registros->ECX[sizeof(registros->ECX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "EDX"))
	{
		strncpy(registros->EDX, nuevo_valor, 8);
		registros->EDX[sizeof(registros->EDX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "RAX"))
	{
		strncpy(registros->RAX, nuevo_valor, 16);
		registros->RAX[sizeof(registros->RAX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "RBX"))
	{
		strncpy(registros->RBX, nuevo_valor, 16);
		registros->RBX[sizeof(registros->RBX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "RCX"))
	{
		strncpy(registros->RCX, nuevo_valor, 16);
		registros->RCX[sizeof(registros->RCX) - 1] = '\0';
	}
	if (string_equals_ignore_case(registro, "RDX"))
	{
		strncpy(registros->RDX, nuevo_valor, 16);
		registros->RDX[sizeof(registros->RDX) - 1] = '\0';
	}

}

char* leerRegistro(char* registro, t_registro* registros){
	
	if(! string_equals_ignore_case(registro,"AX")){
		return registros->AX;
	}
	if(! string_equals_ignore_case(registro,"BX")){
		return registros->BX;
	}
	if(! string_equals_ignore_case(registro,"CX")){
		return registros->CX;
	}
	if(! string_equals_ignore_case(registro,"DX")){
		return registros->DX;
	}
	if(! string_equals_ignore_case(registro,"EAX")){
		return registros->EAX;
	}
	if(! string_equals_ignore_case(registro,"EBX")){
		return registros->EBX;
	}
	if(! string_equals_ignore_case(registro,"ECX")){
		return registros->ECX;
	}
	if(! string_equals_ignore_case(registro,"EDX")){
		return registros->EDX;
	}
	if(! string_equals_ignore_case(registro,"RAX")){
		return registros->RAX;
	}
	if(! string_equals_ignore_case(registro,"RBX")){
		return registros->RBX;
	}
	if(! string_equals_ignore_case(registro,"RCX")){
		return registros->RCX;
	}
	if(! string_equals_ignore_case(registro,"RDX")){
		return registros->RDX;
	}
}

void mostrar_por_pantalla(t_registro registro){
	printf("----------------------------------------------\n");
	printf("valores registros: \n");
	printf("- AX  = %s \n", registro.AX );
	printf("- BX  = %s \n", registro.BX );
	printf("- CX  = %s \n", registro.CX );
	printf("- DX  = %s \n", registro.DX );
	printf("- EAX = %s \n", registro.EAX);
	printf("- EBX = %s \n", registro.EBX);
	printf("- ECX = %s \n", registro.ECX);
	printf("- EDX = %s \n", registro.EDX);
	printf("- RAX = %s \n", registro.RAX);
	printf("- RBX = %s \n", registro.RBX);
	printf("- RCX = %s \n", registro.RCX);
	printf("- RDX = %s \n", registro.RDX);
	printf("----------------------------------------------\n");
}
