#include <cstdlib>
#include <iostream>
#include "pila.h"

#include "lexico.h"
#include "sintactico.h"
#include "semantico.h"

using namespace std;

TablaSimbolos * Nodo::tablaSimbolos= NULL;
string Nodo::ambito= "";

int main(int argc, char *argv[]){

    Sintactico sintactico;
    sintactico.lexico.entrada("int a ; $");
    sintactico.analiza();

    if (sintactico.aceptacion) { // Solo si la sintaxis fue correcta
        Semantico semantico;
        semantico.analiza(sintactico.arbolSintactico); // Analiza el árbol
    }


    system("PAUSE");
    return EXIT_SUCCESS;
}
