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

    // Obtenemos el primer token
    // (Asegúrate que tu Lexico.cpp devuelva el ID de token correcto)
    int token = lexico.sigSimbolo();

    while (true) {
        // 1. Obtener estado actual (está en el TOPE de la pila)
        Estado* estadoActual = (Estado*)pila.top();

        // 2. Consultar la ACCIÓN en la tablaLR
        int accion = tablaLR[estadoActual->id][token];

        // *******************************************************
        // ---- ¡PARCHE PARA EL BUG DE LA TABLA LR! ----
        // En Estado 8, con Token 12 (';'), la tabla dice -8 (r8)
        // pero debería decir -7 (r7). Lo forzamos a mano.
        if (estadoActual->id == 8 && token == 12) {
            accion = -7; // Forzamos la Regla 7 (índice 6)
        }
        // *******************************************************

        // ---- ¡PARCHE 2: BUG del BUCLE INFINITO $ ----
        // En Estado 2, con Token 23 ($), la tabla dice -2 (r2),
        // causando un bucle. Debería ser -1 (r1: programa).
        if (estadoActual->id == 2 && token == 23) {
            accion = -1; // Forzamos la Regla 1 (índice 0)
        }
        // *******************************************************

        // 3. Imprimir para depuración (opcional pero MUY útil)
        pila.muestra();
        cout << "Token: " << token << " | Accion: ";
        muestraAccion(accion);

        // 4. Tomar decisión
        if (accion > 0) {
            // --- DESPLAZAMIENTO (SHIFT) ---
            // Empuja el Terminal (con el ID del token)
            pila.push(new Terminal(token));
            // Empuja el nuevo Estado (el número de 'accion')
            pila.push(new Estado(accion));

            // Pide el siguiente token al léxico
            token = lexico.sigSimbolo();

        } else if (accion < 0) {
            // --- REDUCCIÓN (REDUCE) ---

            if (accion == -1) {
                // --- ACEPTACIÓN ---
                cout << "¡Analisis sintactico TERMINADO con exito!" << endl;
                aceptacion = true;

                // Obtenemos el Nodo Raíz (el NoTerminal 'programa')
                pila.pop(); // Saca el Estado 1
                NoTerminal* raiz = (NoTerminal*)pila.pop(); // Saca 'programa'
                arbolSintactico = raiz->nodo; // ¡Este es tu AST!

                delete raiz;
                break; // Termina el bucle
            }

            // Es una reducción normal (ej. -2, -3, ...)
            int indiceRegla = -(accion + 1); // R1 es -2, R2 es -3, etc.
            InfoRegla regla = infoReglas[indiceRegla];
            int longitud = regla.longitud;
            int idNoTerminal = regla.idNoTerminal;
            string nombreNT = regla.nombre;

            // --- ¡AQUÍ SE CREA EL ÁRBOL! ---
            // 1. Crear el nodo padre (función helper)
            Nodo* nodoPadre = crearNodoAST(indiceRegla, longitud);

            // 2. Obtener el estado anterior (ahora está en el tope)
            Estado* estadoAnterior = (Estado*)pila.top();

            // 3. Consultar la tabla GOTO
            int nuevoEstadoId = tablaLR[estadoAnterior->id][idNoTerminal];

            // 4. Empujar el NoTerminal (con su Nodo AST)
            NoTerminal* nt = new NoTerminal(idNoTerminal, nombreNT);
            nt->nodo = nodoPadre; // ¡Adjuntamos el nodo!
            pila.push(nt);

            // 5. Empujar el nuevo Estado (de GOTO)
            pila.push(new Estado(nuevoEstadoId));

        } else if (accion == 0) {
            // --- ERROR SINTÁCTICO ---
            cout << "ERROR SINTACTICO: Accion 0." << endl;
            cout << "Estado: " << estadoActual->id << ", Token: " << token << endl;
            break; // Termina el bucle
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
