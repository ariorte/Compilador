/*
 *	Analizador sintactico descendente recursivo
 * 	Reglas a seguir:
 *  JSON -> element EOF
 *  element -> object | array
 *  array -> [ a'
 *  a' -> element-list ] | ]
 *  element-list -> element el'
 *  el' ->  ,element el' | ε
 *  object -> { o'
 *  o' -> attributes-list} | }
 *  attributes-list -> attributes al'
 *  al' -> ,attributes al' | ε
 *  attribute -> att-name : att-value
 *  att-name -> string
 *  att-value -> element | string | number | true | false | null
*/

/*=========Incluye al analizador léxico=============*/
#include "lexico.c"
/*==================================================*/

/*=========Procedimientos como prototipos================*/
void element(int synchset[], int size);
void object(int synchset[], int size);
void obj(int synchset[], int size);
void array(int synchset[], int size);
void arr(int synchset[], int size);
void attributeList(int synchset[], int size);
void attrList(int synchset[], int size);
void elementList(int synchset[], int size);
void elmList(int synchset[], int size);
void attribute(int synchset[], int size);
void attName(int synchset[], int size);
void attValue(int synchset[], int size);
/*========================================================*/
/*
 * Variable de aceptación si el archivo fuente es sintácticamente correcto o no,
 * en donde el primer caso sería de aceptar con valor 1 o en caso contrario 0.
*/
int aceptar = 1;

/*==================Procedimiento que muestra el mensaje de error==========*/
void errorSint(){
    aceptar = 0;
    printf("\nLin %d: Error Sintactico. No se esperaba %s. ", numLinea, t.comp);
}
/*=========================================================================*/

/*================Procedimiento que realiza el match===============*/
void match (int expToken) {
    if (t.compLex = expToken) {
        sigLex();
    }
}
/*=================================================================*/

/*
 * vector: es del tipo array de int.
 * size: es del tipo int.
 * comp: es del tipo int.
 * Función que tiene tres parámetros y se espera que los argumentos sean del
 * mismo tipo, y la misma evaluará si lo recibido en 'comp' es igual a algún
 * a algún valor dentro del array (vector) que se recibe también.
 * Retorna: valor es del tipo int, siendo uno (1) en el caso que se encuentre,
 * en caso contrario 0.
*/
int in(int vector[], int size, int comp){
    int i = 0;
    while (i < size) {
        if (vector[i] == comp) {
            return 1;
        }
        i++;
    }
    return 0;
}

/*
 * synchset: es del tipo array de int.
 * size: es del tipo int.
 * Procedimiento que realiza el llamado al siguiente token de acuerdo a la
 * condición.
*/
void scanTo(int synchset[], int size) {
    while (in(synchset, size, t.compLex) == 0 && t.compLex != EOF) {
        sigLex();
    }
    sigLex();
}

/*===============Procedimiento que realiza el chequeo del input===============*/
void checkInput(int primero[], int siguiente[], int size1, int size2) {
    int vunion[size1+size2];
    int i = 0, j = 0;
    while (i < size1) {
        if (t.compLex == primero[i]) { return;}
        i++;
    }
    i = 0;
    while (i < size1) {
        vunion[i] = primero[i];
        i++;
    }
    while (j < size2) {
        vunion[i] = siguiente[j];
        i++;
        j++;
    }
    errorSint();
    scanTo(vunion, size1+size2);
}
/*============================================================================*/

/*==============Implementación de los prototipos=====================*/
// json -> element
void json(){
    int primero[2] = {L_CORCHETE, L_LLAVE};
    int siguiente[1] = {EOF};
    checkInput(primero, siguiente, 2, 1);
    element(siguiente, 1);
}

// element -> object | array
void element(int synchset[], int size){
    int primero[2] = {L_CORCHETE, L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 2, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE) {
            object(siguiente, 4);
        }
        else if (t.compLex == L_CORCHETE) {
            array(siguiente, 4);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 2);
}

// object -> { obj
void object(int synchset[], int size){
    int primero[1] = {L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE) {
            match(L_LLAVE);
            obj(siguiente, 4);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 1);
}

// obj -> attributeList} | }
void obj(int synchset[], int size){
    int primero[2] = {R_LLAVE, STRING};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 2, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == R_LLAVE) {
            match(R_LLAVE);
        }
        else if (t.compLex == STRING) {
            attributeList(siguiente, 4);
            match(R_LLAVE);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 2);
}

// array -> [ arr 
void array(int synchset[], int size){
    int primero[1] = {L_CORCHETE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_CORCHETE) {
            match(L_CORCHETE);
            arr(siguiente, 4);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 4, 1);
}

// arr -> elementList] | ]
void arr(int synchset[], int size){
    int primero[3] = {R_CORCHETE, L_CORCHETE, L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    checkInput(primero, synchset, 3, size);
    if (t.compLex == R_CORCHETE) {
        match(R_CORCHETE);
    }
    else if (t.compLex == L_LLAVE || t.compLex == L_CORCHETE) {
        elementList(siguiente, 4);
        match(R_CORCHETE);
    }
    checkInput(siguiente, primero, 4, 3);
}

// attributeList -> attribute attrList
void attributeList(int synchset[], int size){
    int primero[1] = {STRING};
    int siguiente[1] = {R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == STRING) {
            attribute(siguiente, 1);
                    attrList(siguiente, 1);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 1, 1);
}

// attrList -> , attribute attrList | ES (empty string)
void attrList(int synchset[], int size){
    if (t.compLex == R_LLAVE){
        return;
    }

    int primero[1] = {COMA};
    int siguiente[1] = {R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == COMA) {
            match(COMA);
            attribute(siguiente, 1);
            attrList(siguiente, 1);
        }
    }
    checkInput(siguiente, primero, 1, 1);
}

// elementList -> element elmList
void elementList(int synchset[], int size){
    int primero[2] = {L_LLAVE, L_CORCHETE};
    int siguiente[1] = {R_CORCHETE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 2, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_CORCHETE || t.compLex == L_LLAVE) {
            element(siguiente, 1);
            elmList(siguiente, 1);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 1, 2);
}

// elmList -> , element elmList | ES (empty string)
void elmList(int synchset[], int size){
    if(t.compLex == R_CORCHETE){ 
       return;
    }
    int primero[1] = {COMA};
    int siguiente[1] = {R_CORCHETE};
    checkInput(primero, synchset, 1, size);
    if (t.compLex == COMA) {
        match(COMA);
        element(siguiente, 1);
        elmList(siguiente, 1);
    }
    checkInput(siguiente, primero, 1, 1);
}

// attribute -> attName : attValue
void attribute(int synchset[], int size){
    int primero[1] = {STRING};
    int siguiente[2] = {COMA, R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == STRING) {
            attName(siguiente, 2);
            match(DOS_PUNTOS);
            attValue(siguiente, 2);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 2, 1);
}

// attName -> string
void attName(int synchset[], int size){
    int primero[1] = {STRING};
    int siguiente[1] = {DOS_PUNTOS};
    int i = 0, is = 0;
    checkInput(primero, synchset, 1, size);
    while (i < size) {
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
}
    if (is == 0) {
        if (t.compLex == STRING){
            match(STRING);
        }
    }
    else {
        errorSint();
    }
    checkInput(siguiente, primero, 1, 1);
}

// attValue -> element | string | num | true | false | null
void attValue(int synchset[], int size){
    int primero[7] = {L_LLAVE, L_CORCHETE, STRING, NUMBER, PR_TRUE, PR_FALSE, PR_NULL};
    int siguiente[2] = {COMA, R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, synchset, 7, size);
    while(i < size){
        if (t.compLex == synchset[i]) {
            is = 1;
        }
        i++;
    }
    if(is == 0){
        if(t.compLex == L_LLAVE || t.compLex == L_CORCHETE) {
            element(siguiente, 2);
        }
        
        else if (t.compLex == PR_TRUE) {
            match(PR_TRUE);
        }
        else if (t.compLex == PR_FALSE) {
            match(PR_FALSE);
        }
        else if (t.compLex == PR_NULL) {
            match(PR_NULL);
        }

			else if (t.compLex == STRING) {
            match(STRING);
        }
        else if (t.compLex == NUMBER) {
            match(NUMBER);
        }
}
    else{
        errorSint();
    }
    checkInput(siguiente, primero, 2, 7);
}
/*===================================================================*/
