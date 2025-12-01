#include <cstdlib>
#include <iostream>
#include "pila.h"

#include "lexico.h"
#include "sintactico.h"
#include "semantico.h"
#include "generador.h"

using namespace std;

TablaSimbolos * Nodo::tablaSimbolos= NULL;
string Nodo::ambito= "";

int main(int argc, char *argv[]){

    Sintactico sintactico;
    sintactico.lexico.entrada("int a; float b; int c; $");
    sintactico.analiza();

    if (sintactico.aceptacion) { // Solo si la sintaxis fue correcta
        Semantico semantico;
        semantico.analiza(sintactico.arbolSintactico); // Analiza el árbol

        if (semantico.listaErrores.empty()) { // Solo si no hay errores semánticos
            Generador generador("programa.asm"); // Crea el archivo
            generador.generar(sintactico.arbolSintactico);
        }
    }

    system("PAUSE");
    return EXIT_SUCCESS;
}
