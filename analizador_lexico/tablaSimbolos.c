/*===================Librería(s)==============*/
#include "lexico.h"
/*============================================*/

n aux = NULL;
n nodop = NULL;

/*==============Inserta en la lista el token recibido=============*/
void insertar(token ele){
	n nuevo = (nodo*) malloc(sizeof(nodo));
    
	if (nuevo == NULL) exit(1);
	nuevo->sgte = NULL;
	nuevo->t = ele;
    
	if(nodop == NULL) nodop = nuevo;
	else{
		aux = nodop;
		while(aux->sgte != NULL) aux = aux->sgte;
		aux->sgte = nuevo;
	}
}
/*=================================================================*/

token extraer(){
	aux = nodop;
	nodop = nodop->sgte;
	return aux->t;
}

int esta_vacia(){
    if (nodop == NULL) return 1;
    else return 0;
}

void listar(){
    if (esta_vacia()) printf("la lista se encuentra vacia");
    n aux = nodop;
    while(aux != NULL){
        printf("componentelexico:%d,lexema:%s ,direccion: %p\n",aux->t.complexico,aux->t.lexema,aux->sgte);
        aux = aux->sgte;
    }
}
