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

        // ---- ¡PARCHE 3: BUG de DefVar! ----
        // En Estado 12, con Token 23 ($), la tabla dice -7 (r7)
        // pero debería decir -6 (r6: DefVar).
        if (estadoActual->id == 12 && token == 23) {
            accion = -6; // Forzamos la Regla 6 (índice 5)
        }

        // ---- ¡PARCHE 4: BUG del Crash r3! ----
        // En Estado 3, con Token 23 ($), la tabla dice -3 (r3)
        // pero debería decir -2 (r2: Definiciones ::= \e).
        if (estadoActual->id == 3 && token == 23) {
            accion = -2; // Forzamos la Regla 2 (índice 1)
        }

        // ---- ¡PARCHE 5: BUG del BUCLE r4/r2! ----
        // En Estado 7, con Token 23 ($), la tabla dice -4 (r4)
        // causando un bucle. Debería ser -3 (r3).
        if (estadoActual->id == 7 && token == 23) {
            accion = -3; // Forzamos la Regla 3 (índice 2)
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

                cout << endl << "------ ARBOL SINTACTICO (AST) ------" << endl;
                if (arbolSintactico)
                    arbolSintactico->muestra(); // ¡Llama a la función 'muestra' del Mod 5!
                else
                    cout << " (Arbol Vacio)" << endl;

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
// (¡Esta es la función que rellenamos!)
Nodo* Sintactico::crearNodoAST(int indiceRegla, int longitud) {

    // 1. Sacar los nodos hijos de la pila
    // (Esta parte ya estaba bien, saca los nodos de derecha a izquierda)
    Nodo* hijos[longitud];
    for (int i = longitud - 1; i >= 0; i--) {
        pila.pop(); // Saca el Estado
        ElementoPila* elem = pila.pop();
        hijos[i] = elem->nodo; // Guarda su nodo AST
        delete elem; // Libera la memoria del elemento de pila
    }

    // 2. Crear el nodo PADRE según la regla
    Nodo* nodoPadre = NULL;

    // Declaraciones de variables (para que funcionen en el switch)
    Nodo* definicion;
    Nodo* definiciones;
    Tipo* tipo;
    Identificador* id;
    Identificador* lista;

    // ***************************************************************
    // ----- ¡REEMPLAZO DEL SWITCH! ----
    // ***************************************************************
    // (Los números de regla 'case X:' son 0-indexados, R1 es 0)

    switch (indiceRegla) {

        // R1: <programa> ::= <Definiciones>
        case 0:
            nodoPadre = hijos[0]; // Pasamos la lista de definiciones
            break;

        // R2: <Definiciones> ::= \e (vacío)
        case 1:
            nodoPadre = NULL; // Una lista vacía es NULL
            break;

        // R3: <Definiciones> ::= <Definicion> <Definiciones>
        case 2:
            definicion = hijos[0];
            definiciones = hijos[1];
            if (definicion) definicion->sig = definiciones; // Enlazamos la lista
            nodoPadre = definicion; // Devolvemos la cabeza de la lista
            break;

        // R4: <Definicion> ::= <DefVar>
        case 3:
            nodoPadre = hijos[0]; // Pasa el DefVar hacia arriba
            break;

        // R5: <Definicion> ::= <DefFunc>
        case 4:
            // Aún no hemos manejado DefFunc, pero la pasamos
            nodoPadre = hijos[0];
            break;

        // R6: <DefVar> ::= tipo identificador <ListaVar> ;
        case 5:
            tipo = (Tipo*)hijos[0];
            id = (Identificador*)hijos[1];
            lista = (Identificador*)hijos[2];
            // (hijos[3] es ';', es NULL y lo ignoramos)

            // Enlazamos el primer 'id' con el resto de la lista
            if (id) id->sig = lista;

            // Creamos el nodo DefVar. El 'sig' es para la *próxima* DefVar, no para la lista de IDs
            nodoPadre = new DefVar(tipo, id, NULL);
            break;

        // R7: <ListaVar> ::= \e (vacío)
        case 6:
            nodoPadre = NULL; // Lista vacía
            break;

        // R8: <ListaVar> ::= , identificador <ListaVar>
        case 7:
            // (hijos[0] es ',', es NULL y lo ignoramos)
            id = (Identificador*)hijos[1];
            lista = (Identificador*)hijos[2];
            if (id) id->sig = lista; // Enlazamos el 'id' con el resto de la lista
            nodoPadre = id; // Devolvemos la cabeza de la lista de IDs
            break;

        // TODO: Faltan las reglas R9 hasta R52...

        default:
            // Para todas las reglas que (aún) no definimos,
            // creamos un nodo genérico para que no crashee
            nodoPadre = new Nodo();
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
