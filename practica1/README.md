# Práctica 1 — Analizador Léxico

En esta práctica se desarrolla un **analizador léxico** que reconoce:

- Identificadores
- Enteros y reales
- Palabras reservadas (`if`, `while`, `return`, `else`, `int`, `float`, `void`)
- Operadores de suma, multiplicación, asignación, relacionales, lógicos
- Paréntesis, llaves, punto y coma, coma
- Símbolo de fin de entrada `$`

El analizador debe devolver el **tipo de token** según la tabla indicada por el profesor.

## Archivos esperados
- `lexico.h` — definición de la clase `Lexico`.
- `lexico.cpp` — implementación del analizador léxico.
- `pruebas.cpp` — programa de prueba para validar diferentes entradas.

## Ejemplos de prueba
- `int x = 10;` → tokens: tipo, identificador, =, entero, ;
- `a+b*c` → identificador, +, identificador, *, identificador