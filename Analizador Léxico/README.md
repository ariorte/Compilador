# AnalizadorLexicoParaJson - TRABAJO PRÁCTICO Nº 1 DE COMPILADORES 
# Realizado por:
JOSÉ FABRICIO AQUINO PANDO - 4791781 - SEPTIEMBRE 2017
ADALBERTO ARIEL ORTEGA CARDOZO - 4718818 - SEPTIEMBRE 2017


El programa fue realizado en lenguaje programación "C", este lee un archivo fuente hecho en JSON "fuente.txt", el cual detecta los tokens que se encuentran por linea y a su vez verifica a cual componente léxico pertenece el token actual. Guarda en un archivo "output.txt" a qué componente léxico pertenece e ir separado por espacio.

INSTRUCCIONES PARA EL USO DEL PROGRAMA
**************************************

# PARA COMPILAR:
gcc json.c json.h lista_simbolos.c -o jsonexe

# PARA EJECUTAR:
./jsonexe fuente.txt
