# Seminario de Traductores de Lenguajes II 🖥️

**Estudiante:** Daniel Gaitan Chavez  
**Código:** 219294005  
**Carrera:** Ingeniería en Computación (INCO)  
**Ciclo:** 2025B  
**Institución:** Universidad de Guadalajara (CUCEI)

---

## 📖 Descripción del Curso
Este repositorio documenta el proceso completo de diseño e implementación de un **Compilador Funcional** y diversas herramientas de traducción de lenguajes. A lo largo del semestre, se desarrollaron desde componentes básicos (analizadores léxicos) hasta un compilador completo capaz de generar código ensamblador (ASM), integrando además tecnologías modernas de validación de datos.

---

## 📂 Estructura del Proyecto

El trabajo se divide en dos grandes módulos:

### 1. 🚀 Proyecto Final (Compilador Completo)
Ubicación: `/ProyectoCompilador`

Este es el núcleo del curso. Se trata de un compilador desarrollado en **C++** que implementa las 4 fases clásicas de la traducción:

* **Fase 1: Análisis Léxico (Scanner):** Tokenización del código fuente y manejo de tabla de símbolos.
* **Fase 2: Análisis Sintáctico (Parser):** Implementación de un autómata de pila **LR(1)** guiado por tabla para validar la gramática.
* **Fase 3: Análisis Semántico:** Validación de tipos, ámbitos de variables y consistencia lógica.
* **Fase 4: Generación de Código:** Traducción del árbol sintáctico (AST) a código ensamblador compatible con **Emu8086**.

**Características Técnicas:**
* **Arquitectura:** Híbrida (Motor en C++ / Pre-procesamiento de tablas en Python).
* **Manejo de Memoria:** Uso de pilas polimórficas y árboles dinámicos.
* **Entrada:** Lectura de archivos fuente `.txt`.

---

### 2. 📚 Prácticas y Tareas
Ubicación: `/Practicas y tareas`

Colección de ejercicios y prototipos desarrollados durante el aprendizaje incremental:

* **Mini Generadores:** Versiones preliminares de escáneres léxicos.
* **Analizadores Sintácticos:** Implementaciones de algoritmos LR con pilas de enteros y objetos.
* **Validación de Datos (Node.js):**
    * Implementación de un sistema de validación de reglas de negocio utilizando **JSON Schema** y la librería **Ajv**.
    * Caso de uso real: Validación de órdenes de servicio para "Autocristales Gaitán".
* **Gramáticas:** Documentación formal (BNF) y scripts ETL para el procesamiento de tablas gramaticales.

---

## 🛠️ Tecnologías Utilizadas

| Tecnología | Uso en el Proyecto |
| :--- | :--- |
| **C++ (STL)** | Desarrollo del motor del compilador (Lexico, Sintáctico, Semántico). |
| **Python (Pandas)** | Scripting para limpieza y transformación de tablas LR (.csv a matriz). |
| **Node.js + Ajv** | Módulo de validación de datos y prototipado de reglas de negocio. |
| **Markdown** | Documentación técnica y reportes de laboratorio. |
| **Git / GitHub** | Control de versiones y portafolio de evidencias. |

---

## 🎓 Conclusión del Curso
Este seminario permitió consolidar los conocimientos teóricos sobre la teoría de lenguajes formales y autómatas, llevándolos a la práctica mediante la construcción de software complejo. Se superaron retos significativos como la gestión de tablas de análisis corruptas, la integración de múltiples lenguajes de programación y la depuración de estructuras de datos recursivas (AST).

---
*Daniel Gaitan - 2025*
