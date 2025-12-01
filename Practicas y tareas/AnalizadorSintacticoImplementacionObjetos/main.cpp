#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// ==========================================
// 1. JERARQUÍA DE CLASES (ElementoPila)
// ==========================================

// Clase Base Abstracta
class ElementoPila {
public:
    virtual void mostrar() = 0; // Método virtual puro
    virtual ~ElementoPila() {}  // Destructor virtual
};

// Clase para tokens (id, +, $)
class Terminal : public ElementoPila {
public:
    string lexema;
    Terminal(string l) : lexema(l) {}
    void mostrar() override {
        cout << lexema << " ";
    }
};

// Clase para No Terminales (E)
class NoTerminal : public ElementoPila {
public:
    string nombre;
    NoTerminal(string n) : nombre(n) {}
    void mostrar() override {
        cout << nombre << " ";
    }
};

// Clase para Estados (0, 1, 2...)
class Estado : public ElementoPila {
public:
    int id;
    Estado(int i) : id(i) {}
    void mostrar() override {
        cout << id << " ";
    }
};

// ==========================================
// 2. CLASE PILA (De Objetos)
// ==========================================
class Pila {
private:
    vector<ElementoPila*> elementos;
public:
    void push(ElementoPila* x) {
        elementos.push_back(x);
    }

    ElementoPila* pop() {
        if (elementos.empty()) return nullptr;
        ElementoPila* top = elementos.back();
        elementos.pop_back();
        return top;
    }

    ElementoPila* top() {
        if (elementos.empty()) return nullptr;
        return elementos.back();
    }

    // Devuelve el ID del estado en el tope (helper para la tabla LR)
    int obtenerEstadoTope() {
        if (elementos.empty()) return -1;
        // Casteamos dinámicamente para asegurar que sea un Estado
        Estado* e = dynamic_cast<Estado*>(elementos.back());
        if (e) return e->id;
        return -1; // Error: el tope no era un estado
    }

    void muestra() {
        cout << "Pila: [ ";
        for (ElementoPila* e : elementos) {
            e->mostrar();
        }
        cout << "]" << endl;
    }
};

// ==========================================
// 3. CLASE LEXICO (Simulada)
// ==========================================
class Lexico {
private:
    string entrada;
    int indice;
public:
    int tipo;     // 0=id, 1=+, 2=$
    string lexema;

    Lexico(string in) {
        entrada = in;
        indice = 0;
    }

    void sigSimbolo() {
        if (indice >= entrada.length()) {
            lexema = "$";
            tipo = 2; 
            return;
        }

        char c = entrada[indice++];
        lexema = string(1, c);

        if (isalpha(c)) tipo = 0;      // id
        else if (c == '+') tipo = 1;   // +
        else tipo = 2;                 // $ (default)
    }
};

// ==========================================
// 4. MAIN - ANALIZADOR LR CON OBJETOS
// ==========================================
int main() {
    // Gramática: E -> id + E | id
    // Tabla LR del Ejercicio 2
    int tablaLR[5][4] = {
        { 2,  0,  0,  1}, // Est 0
        { 0,  0, -1,  0}, // Est 1 (Aceptación)
        { 0,  3, -3,  0}, // Est 2 (r2: E->id)
        { 2,  0,  0,  4}, // Est 3
        { 0,  0, -2,  0}  // Est 4 (r1: E->id+E)
    };

    Pila pila;
    Lexico lexico("a+b+c");
    int fila, columna, accion;
    bool aceptacion = false;

    // Inicialización: Estado 0
    pila.push(new Estado(0));
    lexico.sigSimbolo();

    cout << "--- ANALISIS SINTACTICO (PILA DE OBJETOS) ---" << endl;

    while (!aceptacion) {
        // 1. Obtener fila (Estado en el tope)
        fila = pila.obtenerEstadoTope();
        
        // 2. Obtener columna (Token actual)
        columna = lexico.tipo;

        // 3. Buscar acción
        accion = tablaLR[fila][columna];

        // Mostrar estado actual
        pila.muestra();
        cout << "Entrada: " << lexico.lexema << " | Accion: " << accion << endl;
        cout << "--------------------------------" << endl;

        // --- DESPLAZAMIENTO (SHIFT) ---
        if (accion > 0) {
            pila.push(new Terminal(lexico.lexema)); // Guardamos el token real ("a")
            pila.push(new Estado(accion));          // Guardamos el nuevo estado
            lexico.sigSimbolo();
        }
        
        // --- REDUCCION (REDUCE) ---
        else if (accion < 0) {
            if (accion == -1) {
                cout << ">>> CADENA ACEPTADA <<<" << endl;
                aceptacion = true;
                break;
            }

            // Regla 1 (E -> id + E): Pop 3 símbolos * 2 (obj+estado) = 6
            if (accion == -2) {
                cout << "Reduccion: E -> id + E" << endl;
                for(int i=0; i<6; i++) pila.pop(); 
            }
            // Regla 2 (E -> id): Pop 1 símbolo * 2 (obj+estado) = 2
            else if (accion == -3) {
                cout << "Reduccion: E -> id" << endl;
                pila.pop(); // estado
                pila.pop(); // terminal "id"
            }

            // --- GOTO ---
            // El tope ahora debe ser un estado previo
            fila = pila.obtenerEstadoTope();
            columna = 3; // Columna de No Terminal E
            int estado_goto = tablaLR[fila][columna];

            cout << "GOTO(" << fila << ", E) -> " << estado_goto << endl;

            pila.push(new NoTerminal("E"));       // Metemos el No Terminal
            pila.push(new Estado(estado_goto));   // Metemos el estado resultante
        }
        else {
            cout << "Error de Sintaxis." << endl;
            break;
        }
    }

    return 0;
}