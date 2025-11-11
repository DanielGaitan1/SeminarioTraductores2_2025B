#include "arbolSintactico.h"
#include "generador.h" // <-- ¡LA MAGIA! Ahora sí podemos incluir esto.

// --- Definición de la variable estática ---
int Nodo::sangria = 0;

// (El .h nuevo ya define 'muestraSangria()', así que no va aquí)

// ************************************
// ---- ¡AQUÍ ESTÁN LAS "RECETAS"! ----
// ************************************

void Identificador::generaCodigo(Generador* gen) {
    // Cuando un 'id' es usado en una expresión,
    // queremos que ponga su valor en el registro AX.
    gen->emit("MOV AX, " + simbolo);
}

void DefVar::generaCodigo(Generador* gen) {
    // La definición de una variable no genera CÓDIGO, solo DATOS.

    // Llama al siguiente nodo (la próxima definición)
    if (sig) sig->generaCodigo(gen);
}

void DefVar::generaDatos(Generador* gen) {
    // 1. Genera el código para las variables
    Identificador *p = listaVar;
    while (p != NULL) {
        // Escribe en el archivo: "a DW 0" (Define Word)
        gen->emitData(p->simbolo + " DW 0");
        p = (Identificador*)p->sig;
    }

    // 2. Llama al siguiente nodo (la próxima definición)
    if (sig) sig->generaDatos(gen);
}
