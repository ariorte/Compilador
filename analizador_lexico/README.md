# Analizador Lexico Para Json - TRABAJO PRÁCTICO Nº 1 DE COMPILADORES 
# Realizado por:
ADALBERTO ARIEL ORTEGA CARDOZO


El programa fue realizado en lenguaje programación "C", este lee un archivo fuente hecho en JSON "fuente.txt", el cual detecta los tokens que se encuentran por linea y a su vez verifica a cual componente léxico pertenece el token actual. Guarda en un archivo "output.txt" a qué componente léxico pertenece e ir separado por espacio.

INSTRUCCIONES PARA EL USO DEL PROGRAMA
**************************************

# PARA COMPILAR:
gcc lexico.c tablaSimbolos.c -o lexico

# PARA EJECUTAR:
./lexico <ubicacion del archivo fuente con extension .txt>
