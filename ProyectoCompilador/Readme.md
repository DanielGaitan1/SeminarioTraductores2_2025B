# Proyecto Final: Compilador (Seminario de Traductores 2)

Este repositorio contiene el proyecto final para la materia Seminario de Traductores de Lenguaje 2. Es un compilador funcional de 3 etapas construido desde cero en C++.

## 📈 Estado Actual (Avance: 50%)

El proyecto implementa exitosamente las primeras tres fases de un compilador:

1.  **✅ Analizador Léxico 
2.  **✅ Analizador Sintáctico 
3.  **✅ Análisis Semántico 

La etapa final, **Generación de Código**, está pendiente.

---

## 🛠️ 1. Analizador Léxico

* Carga un archivo `.inf` en un `std::map` al iniciar.
* Tokeniza el código fuente, distinguiendo correctamente entre **palabras clave** (como `int`, `if`, etc.) y **categorías** (como `tipo` o `identificador`).
* Provee el *string* del token (ej. `"a"`) al analizador sintáctico para la construcción del AST.

## ⚙️ 2. Analizador Sintáctico (Motor LR(1) y AST)

Esta es la etapa más compleja del proyecto.

### Motor LR(1)
* Implementa un motor de parsing LR(1) completo basado en una pila.
* Carga la tabla `LR(1)` y las reglas de la gramática desde un archivo `.lr` externo.
* **Depuración de la Tabla:** Durante la implementación, se detectaron y corrigieron 5 conflictos críticos (reducción-reducción y reducción-desplazamiento) en el archivo `.lr` proporcionado. Estos se solucionaron mediante "parches" lógicos en el motor para forzar las acciones gramaticales correctas y evitar bucles infinitos o *crashes*.

### Construcción del Árbol de Sintaxis Abstracta (AST)
El motor construye un Árbol de Sintaxis Abstracta (AST) funcional usando el siguiente método:

1.  **Shift (Desplazar):** Al desplazar un token, se crea el nodo "hoja" correspondiente (ej. `new Tipo("int")` o `new Identificador("a")`) y se enlaza al `Terminal` en la pila.
2.  **Reduce (Reducir):** Al reducir, la función `crearNodoAST` toma los nodos "hijo" ya creados, los saca de la pila y los ensambla en un nuevo nodo "padre" (ej. `new DefVar(...)`).

## 💡 3. Análisis Semántico

* Implementa un recorrido `virtual void validaTipos()` sobre el AST (Patrón Visitor).
* **Tabla de Símbolos:** Rellena una `TablaSimbolos` (basada en Hash) con las definiciones de variables (`DefVar`), gestionando ámbitos (global/local) y detectando variables redefinidas.
* **Chequeo de Tipos:** Valida la coherencia de tipos en operaciones (ej. `Suma`, `Mult`) y asignaciones.

---

## 📋 Próximos Pasos (Generación de Código)

La etapa final es implementar la función `virtual void generaCodigo()` en el AST. El plan es "traducir" el árbol semánticamente validado a un lenguaje objetivo (Ensamblador o Python), generando un archivo de salida ejecutable.