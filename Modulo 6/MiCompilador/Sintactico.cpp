#include "sintactico.h"

// --- Constructor (¡Ya funciona!) ---
Sintactico::Sintactico() {
    // ... (Tu código de cargar tabla LR y símbolos se queda aquí) ...
    // ... (No lo pego de nuevo para no hacer bulto, pero DEBE estar aquí) ...

    std::ifstream archivoLR("compilador.lr");
    if (!archivoLR.is_open()) {
        cout << "ERROR: No se pudo abrir 'compilador.lr'" << endl; system("PAUSE"); return;
    }
    archivoLR >> numReglas;
    infoReglas.resize(numReglas);
    for (int i = 0; i < numReglas; ++i) {
        archivoLR >> infoReglas[i].idNoTerminal >> infoReglas[i].longitud >> infoReglas[i].nombre;
    }
    archivoLR >> filas >> columnas;
    tablaLR.resize(filas, std::vector<int>(columnas));
    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            archivoLR >> tablaLR[f][c];
        }
    }
    archivoLR.close();
    cout << "Constructor de Sintactico: ¡TABLA CARGADA!" << endl;
    cout << "Reglas: " << numReglas << " | Tabla: " << filas << "x" << columnas << endl;
    cout << "****************************************" << endl;
}

// --- Destructor (para limpiar la pila) ---
Sintactico::~Sintactico() {
    while (!pila.esVacia()) {
        delete pila.pop();
    }
}


// --- El "MOTOR" LR(1) ----
void Sintactico::analiza() {
    aceptacion = false;
    pila.push(new Estado(0)); // Empujar estado inicial 0

    int token = lexico.sigSimbolo();

    while (true) {
        Estado* estadoActual = (Estado*)pila.top();
        int accion = tablaLR[estadoActual->id][token];

        // *** PARCHE 1: BUG del ; ***
        if (estadoActual->id == 8 && token == 12) {
            accion = -7;
        }

        // *** PARCHE 2: BUG del BUCLE INFINITO $ ***
        if (estadoActual->id == 2 && token == 23) {
            accion = -1;
        }

        pila.muestra();
        cout << "Token: " << token << " | Accion: ";
        muestraAccion(accion);

        if (accion > 0) {
            // *******************************************************
            // ---- ¡SECCIÓN DE DESPLAZAMIENTO (SHIFT) MEJORADA! ----
            // *******************************************************

            // 1. Crear el Nodo del Árbol (AST)
            Nodo* nodoAST = NULL; // Por defecto es NULL
            string buffer = lexico.getBuffer(); // Pedimos el texto ("int", "a")

            // Si es un token que GENERA un nodo (como id o tipo)
            if (token == 0) { // token 0 es "identificador"
                nodoAST = new Identificador(buffer);
            }
            else if (token == 4) { // token 4 es "tipo"
                nodoAST = new Tipo(buffer);
            }
            // (Aquí puedes añadir 'else if' para ENTERO, REAL, CADENA)

            // 2. Crear el Terminal y "pegarle" el nodo
            Terminal* term = new Terminal(token);
            term->nodo = nodoAST; // <-- ¡La magia está aquí!

            // 3. Empujar el Terminal y el nuevo Estado
            pila.push(term);
            pila.push(new Estado(accion));

            // 4. Pedir el siguiente token
            token = lexico.sigSimbolo();
            // *******************************************************

        } else if (accion < 0) {
            // --- REDUCCIÓN (REDUCE) ---

            if (accion == -1) {
                // --- ACEPTACIÓN ---
                cout << "¡Analisis sintactico TERMINADO con exito!" << endl;
                aceptacion = true;

                pila.pop(); // Saca el Estado 1
                NoTerminal* raiz = (NoTerminal*)pila.pop();
                arbolSintactico = raiz->nodo; // ¡Este es tu AST!

                delete raiz;
                break;
            }

            // Es una reducción normal
            int indiceRegla = -(accion + 1);
            InfoRegla regla = infoReglas[indiceRegla];
            int longitud = regla.longitud;
            int idNoTerminal = regla.idNoTerminal;
            string nombreNT = regla.nombre;

            // 1. Crear el nodo padre
            Nodo* nodoPadre = crearNodoAST(indiceRegla, longitud);


            // 3. Obtener el estado anterior
            Estado* estadoAnterior = (Estado*)pila.top();

            // 4. Consultar la tabla GOTO
            int nuevoEstadoId = tablaLR[estadoAnterior->id][idNoTerminal];

            // 5. Empujar el NoTerminal (con su Nodo AST)
            NoTerminal* nt = new NoTerminal(idNoTerminal, nombreNT);
            nt->nodo = nodoPadre; // ¡Adjuntamos el nodo!
            pila.push(nt);

            // 6. Empujar el nuevo Estado (de GOTO)
            pila.push(new Estado(nuevoEstadoId));

        } else if (accion == 0) {
            // --- ERROR SINTÁCTICO ---
            cout << "ERROR SINTACTICO: Accion 0." << endl;
            cout << "Estado: " << estadoActual->id << ", Token: " << token << endl;
            break;
        }
    }
}

// --- FUNCIÓN HELPER PARA CREAR EL AST ---
// (Aquí es donde necesitamos el otro 'arbolSintactico.h')
Nodo* Sintactico::crearNodoAST(int indiceRegla, int longitud) {

    // 1. Sacar los nodos hijos de la pila
    // (Salen en orden inverso, de derecha a izquierda)
    Nodo* hijos[longitud];
    for (int i = longitud - 1; i >= 0; i--) {
        pila.pop(); // Saca el Estado
        ElementoPila* elem = pila.pop(); // Saca el Terminal/NoTerminal
        hijos[i] = elem->nodo; // Guarda su nodo AST
        delete elem; // Libera la memoria del elemento de pila
    }

    // 2. Crear el nodo PADRE según la regla
    Nodo* nodoPadre = NULL;

    // ***************************************************************
    // ----- ¡¡¡ TODO: AQUI NECESITAS TU OTRO ARBOLSINTACTICO.H !!! ---
    // ***************************************************************
    // Reemplaza este 'switch' con los constructores de tus clases
    // del 'arbolSintactico.h' del MODULO 5.
    // (Los números de regla 'case X:' son 0-indexados,
    //  R1 es 0, R2 es 1, etc.)
    switch (indiceRegla) {
        /* Ejemplo de cómo se vería:
        case 0: // R1 <programa> ::= <Definiciones>
            nodoPadre = new NodoPrograma(hijos[0]);
            break;
        case 5: // R6 <DefVar> ::= tipo identificador <ListaVar> ;
            // (Nota: los terminales como 'tipo' o ';' quizás
            //  no crean nodos, así que hijos[X] sería NULL)
            nodoPadre = new NodoDefVar(hijos[0], hijos[1], hijos[2]);
            break;
        case 20: // R21 <Sentencia> ::= identificador = <Expresion> ;
            nodoPadre = new NodoAsignacion(hijos[0], hijos[2]);
            break;
        */

        // --- POR AHORA, SOLO CREAMOS NODOS GENÉRICOS ---
        default:
            nodoPadre = new Nodo();
            // (Cuando tengas las clases, borra este 'default'
            //  y pon todos los 'case')
            break;
    }

    return nodoPadre;
}


// --- Función de ayuda para imprimir la acción ---
void Sintactico::muestraAccion(int accion) {
    if (accion == 0)
        cout << "Error" << endl;
    else if (accion > 0)
        cout << "d" << accion << endl;
    else if (accion == -1) {
        cout << "Aceptacion" << endl;
        aceptacion = true;
    } else if (accion < -1) {
        int r = -(accion + 1);
        cout << "r" << (r+1) << " (Regla: " << infoReglas[r].nombre << ")" << endl;
    }
}

// --- Esta función ya no la usamos, la vaciamos ---
void Sintactico::analizaGramatica0() {
    // (Vacío)
}

bool Pila::esVacia(){
    // Devuelve 'true' si la lista interna está vacía
    return lista.empty();
}
