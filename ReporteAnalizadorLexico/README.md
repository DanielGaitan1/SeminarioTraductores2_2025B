# SeminarioTraductores2
Repositorio para la clase de Seminario de Traductores 2 

# Analizador Léxico - Proyecto Seminario Traductores 2

Este es un analizador léxico que forma parte del seminario de traductores 2. El objetivo del proyecto es implementar un analizador léxico que sea capaz de leer código fuente y clasificar los diferentes elementos según sus tipos. El programa está diseñado para reconocer identificadores, operadores, tipos de datos, y palabras clave de un lenguaje ficticio basado en C.

Descripcion:
Este proyecto implementa un analizador léxico en Python como parte de un compilador. Su tarea principal es leer un código fuente en lenguaje de tipo C (que incluye variables, operaciones, estructuras de control, etc.) y descomponerlo en tokens que representen los elementos básicos del lenguaje, como identificadores, operadores, palabras clave y otros simbolos. Los tokens se clasifican según tipos específicos definidos en la gramática, y cada token es asignado a un valor único.

El analizador léxico es la primera fase de un compilador y tiene como objetivo transformar el código fuente en una saecuencia de tokens que pueden ser procesados por el siguiente paso del compilador (por ejemplo, un analizador sintáctico).


Requisitos: 
  Python3
  Dependencias: re

Instrucciones de Ejecucion:

python analizador.py

Ejemplo de entrada
int a = 5;
float b = 3.14;
if(a > b){
    return a + b;
} else {
    while (a != 0){
        a = a - 1;
    }
}

Ejemplo de salida:
('int', 4)
('a', 0)
('=', 18)
('5', 1)
(';', 12)
('float', 4)
('b', 0)
('=', 18)
('3.14', 2)
(';', 12)
('if', 19)
('(', 14)
('a', 0)
('>', 7)
('b', 0)
(')', 15)
('{', 16)
('return', 21)
('a', 0)
('+', 5)
('b', 0)
(';', 12)
('}', 17)
('else', 22)
('{', 16)
('while', 20)
('(', 14)
('a', 0)
('!=', 11)
('0', 1)
(')', 15)
('{', 16)
('a', 0)
('=', 18)
('a', 0)
('-', 5)
('1', 1)
(';', 12)
('}', 17)
('}', 17)
('$' , 23)

![Analizador1](https://github.com/user-attachments/assets/c79eadf5-a978-4a0d-8f57-fac7202f265e)
![analizador2](https://github.com/user-attachments/assets/66dff6f6-5a0d-40cc-b245-b8548d6ca8fc)
![analizador3](https://github.com/user-attachments/assets/a458df75-e743-4542-ad73-51bebdd8b0ae)
![analizador 4](https://github.com/user-attachments/assets/fd585b46-4833-4135-87d6-d4954028f359)
![analizador 5](https://github.com/user-attachments/assets/f5b6a853-9df7-47a0-a400-3abaff0f3f28)

Funcionamiento del código:

1. Definición de Patrones: En el código se feinen expresiones regulares (regex) que corresponden a los diferentes tipos de tokens que el analizador debe reconocer. Estos patrones cubrenn una amoplia gama de posibles entradas, como:
-Palabras clave (if, while, return, etc.)
-Tipos de datos (int, float)
-Identificadores (nombres de variables)
-Operadores ariméticos y relacionales (+, -, *, >, <=, &&, etc)
-Simbolos de control ({, }, ;, = , (, ),)
-Numeros enteros y reales.

2. Expresion Regular Global:
La funcion analizador_lexico consntruye una expresion regular global que combina todos los patrones definidos. Esto le permite al analizador recorrer el código fuente y encontrar coincidencias con cualquiera de los patrones

3.Lectura del Codigo Fuente: La funcion analizador_lexico recibe el codigo fuente como un texto y comienza a buscar coincidencias con las expresiones regulares predefinidas. A medida que encuentra una coincidencia, la función identifica el tipo de token correspondiente (por ejemplo, si se encuentra una palabra reservada como if, se clasifica como el tipo de token correspondiente a if)

4. Clasificación de Tokens: Para cada coincidencia, el analizadot asigna un tipo de token que corresponde a uno de los tipos definidos en la tabla de especificación de tokens. Cada token es luego guardado en una lista de tokens junto con su tipo (por ejemplo, un identificador de variable podría ser identificado como tipo 0, mientras que un núnero entero podría ser identificado como tipo 1)

5. Manejo de Errores Léxicos: Si el analizador encuentra un símbolo que no coincide con ninguno de los patrones definidos, se considera un error léxico. En tal caso, el programa imprime un mensaje de error indicando que un símbolo desconocido fue encontrado.

6. Resultado final: Al final del analisis léxico, la función devuleve una lista de tuplas, donde cada tupla contiene el valor del token y su tipo correspondiente. Además, se agrega un token especial $ con el tipo 23 para marcar el final de la entrada.





