/*
 *  Analizador Léxico	
 *  Analizador léxico que se utilizará junto al analizador de sintaxis y el traductor.
 *	
 *  Reconoce los componentes lexicos válidos para el lenguaje JSON
 *	
 */

//Incluyo la cabecera
#include "lexer.h"


//Variables globales a ser usadas

token t;// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico
// Fuente pascal
FILE *archivo;
// Buffer para lectura de archivo fuente
char buff[2*TAMBUFF];
// Utilizado por el analizador lexico
char id[TAMLEX];
// Numero de Linea
int numLinea=1;

//Funciones


// Rutinas del analizador lexico

void error(const char* mensaje){
    printf("Lin %d: Error Lexico. %s. ",numLinea,mensaje);	
}


void sigLex(){
    int i=0;
    char c=0;
    int acepto=0;
    int estado=0;
    char msg[41];

    while((c=fgetc(archivo))!=EOF){
        if (c==' ' || c=='\t'){
            continue;//Eliminar espacios en blanco
        }
        else if(c=='\t'){
            continue;//Incremento el numero de linea
        }
        else if(c=='\n'){
            numLinea++;//Incrementar el numero de linea
            continue;
        }
        else if (c==':'){
            t.compLex=DOS_PUNTOS;
            t.comp="DOS_PUNTOS";
				t.lexema=":";
            break;
        }
        else if (c=='{'){
            t.compLex=L_LLAVE;
            t.comp="L_LLAVE";
				t.lexema="{";
            break;
        }
        else if (c=='}'){
            t.compLex=R_LLAVE;
            t.comp="R_LLAVE";
				t.lexema="}";
            break;
        }
        else if (c=='['){
            t.compLex=L_CORCHETE;
            t.comp="L_CORCHETE";
				t.lexema="[";
            break;
        }
        else if (c==']'){
            t.compLex=R_CORCHETE;
            t.comp="R_CORCHETE";
				t.lexema="]";
            break;
        }
        else if (c==','){
            t.compLex=COMA;
            t.comp="COMA";
				t.lexema=",";
            break;
        }
        else if (c == '"'){
            //Es un STRING 
				/*VERIFICA QUE SE INGRESA UN STRING VALIDO*/
            c=fgetc(archivo);
				i = 0;
				id[i]= c; 
            while(c!=EOF){
                if(c == '"'){
						id[i] = '\0';
						t.compLex=STRING;
						t.comp="STRING";
						t.lexema = id;
                  break;
                }else if(i>=TAMLEX){
					ungetc(c, archivo);
				}
				else{
					c=fgetc(archivo);
					id[++i] = c;
                }
            }
            if (c==EOF)
                error("Se ha llegado al fin sin cerrar el String");
			else if(i>=TAMLEX){
				error("La longitud de Identificador excede tamaño de buffer");
			}
            break;
        }
        else if (isdigit(c)){
            //Es un numero
            i=0;
            estado=0;
            acepto=0;
            id[i]=c;

            while(!acepto){
                switch(estado){
                    case 0: //Una secuencia de digitos, puede ocurrir . o e
                        c=fgetc(archivo);
                        if (isdigit(c)){
                            id[++i]=c;
                            estado=0;
                        }
                        else if(c=='.'){
                            id[++i]=c;
                            estado=1;
                        }
                        else if(tolower(c)=='e'){
                            id[++i]=c;
                            estado=3;
                        }
                        else{
                            estado=6;
                        }
                        break;

                    case 1://Un punto, debe seguir un digito (caso especial de array, puede venir otro punto)
                        c=fgetc(archivo);						
                        if (isdigit(c)){
                            id[++i]=c;
                            estado=2;
                        }
                        else if(c=='.'){
                            i--;
                            fseek(archivo,-1,SEEK_CUR);
                            estado=6;
                        }
                        else{
                            sprintf(msg,"No se esperaba '%c'",c);
                            estado=-1;
                        }
                        break;
                    case 2://La fraccion decimal, pueden seguir los digitos o e
                        c=fgetc(archivo);
                        if (isdigit(c)){
                            id[++i]=c;
                            estado=2;
                        }
                        else if(tolower(c)=='e'){
                            id[++i]=c;
                            estado=3;
                        }
                        else
                            estado=6;
                        break;
                    case 3://Una e, puede seguir +, - o una secuencia de digitos
                        c=fgetc(archivo);
                        if (c=='+' || c=='-'){
                            id[++i]=c;
                            estado=4;
                        }
                        else if(isdigit(c)){
                            id[++i]=c;
                            estado=5;
                        }
                        else{
                            sprintf(msg,"No se esperaba '%c'",c);
                            estado=-1;
                        }
                        break;
                    case 4://Es necesario que venga por lo menos un digito
                        c=fgetc(archivo);
                        if (isdigit(c)){
                            id[++i]=c;
                            estado=5;
                        }
                        else{
                            sprintf(msg,"No se esperaba '%c'",c);
                            estado=-1;
                        }
                        break;
                    case 5://Una secuencia de digitos que corresponde al exponente
                        c=fgetc(archivo);
                        if (isdigit(c)){
                            id[++i]=c;
                            estado=5;
                        }
                        else{
                            estado=6;
                        }break;
                    case 6://Estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
                        if (c!=EOF)
                            ungetc(c,archivo);
                        else
                           c=0;
									id[++i]='\0';
									acepto=1;
									t.compLex=NUMBER;
									t.comp="NUMBER";
									t.lexema = id;
                        break;
                    case -1:
                        if (c==EOF)
                            error("No se esperaba el fin de archivo");
                        else
                            error(msg);
                    }
                }
            break;
        }
        else if (isalpha(c)) {
            i = 0;
            do{
                id[i] = c;
                i++;
                c = fgetc(archivo);
            }while (isalpha(c));
            id[i]='\0';
            if (c!=EOF)
                ungetc(c,archivo);
            if(strcmp(id, "true") == 0 || strcmp(id, "TRUE") == 0){
                t.compLex=PR_TRUE;
                t.comp="PR_TRUE";
				t.lexema = "true";
                break;
            }
            else if (strcmp(id, "false") == 0 || strcmp(id, "FALSE") == 0){
                t.compLex=PR_FALSE;
                t.comp="PR_FALSE";
				t.lexema = "false";
                break;
            }
            else if(strcmp(id, "null") == 0 || strcmp(id, "NULL") == 0){
                t.compLex=PR_NULL;
                t.comp="PR_NULL";
				t.lexema = "null";
                break;
            }
            else{
                sprintf(msg,"No se esperaba '%s'",id);
                error(msg);				
            }
        }
        else{
            sprintf(msg,"No se esperaba '%c'",c);
            error(msg);
        }
    }
    if(c==EOF)
    {
        t.compLex=EOF;
        t.comp="EOF";
    }
}

