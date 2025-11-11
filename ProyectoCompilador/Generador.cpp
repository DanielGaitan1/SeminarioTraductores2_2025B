#include "generador.h"

Generador::Generador(const char* nombreArchivo) {
    archivo.open(nombreArchivo);
}

Generador::~Generador() {
    if (archivo.is_open()) {
        archivo.close();
    }
}

// Escribe en la sección de código
void Generador::emit(string codigo) {
    archivo << "    " << codigo << endl;
}

// Escribe en la sección de datos
void Generador::emitData(string codigo) {
    archivo << codigo << endl;
}

// Función principal que inicia todo
void Generador::generar(Nodo* arbol) {
    // Encabezado estándar de emu8086
    archivo << "MODEL SMALL" << endl;
    archivo << ".STACK 100h" << endl;

    // 1. Sección de Datos
    archivo << ".DATA" << endl;
    if (arbol) {
        // (Aquí le diremos al árbol que genere sus datos)
         arbol->generaDatos(this);
    }

    // 2. Sección de Código
    archivo << ".CODE" << endl;
    archivo << "START:" << endl;
    archivo << "    MOV AX, @DATA" << endl; // Inicializar DS
    archivo << "    MOV DS, AX" << endl;

    if (arbol) {
        arbol->generaCodigo(this); // ¡Llama al árbol!
    }

    // Fin del programa
    archivo << "    MOV AX, 4C00h" << endl; // Salir
    archivo << "    INT 21h" << endl;
    archivo << "END START" << endl;

    cout << endl << "------ GENERACION DE CODIGO ------" << endl;
    cout << "¡Archivo 'programa.asm' generado con exito!" << endl;
}
