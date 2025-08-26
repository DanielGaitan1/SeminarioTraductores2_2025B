# Práctica 3 — Analizador Sintáctico LR(1) con Pila de Objetos

En esta práctica se modifica el analizador LR(1) de la práctica anterior para usar una **pila de objetos** en lugar de enteros.

## Objetivos
- Crear una clase base `ElementoPila`.
- Implementar clases que hereden de ella:
  - `Terminal`
  - `NoTerminal`
  - `Estado`
- Modificar la pila para manejar objetos `ElementoPila*`.

## Archivos esperados
- `pila.h` — definición de la pila con objetos.
- `pila.cpp` — implementación de métodos (push, pop, top, muestra).
- `main.cpp` — contiene ejemplos y pruebas.

## Notas
- El comportamiento debe ser equivalente a la práctica 2.
- Al imprimir la pila, ahora deben mostrarse los símbolos de manera legible, similar al análisis manual.

![Ejecución ejemplo objetos](img1.png)
