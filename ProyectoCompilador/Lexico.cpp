#include "lexico.h"

// --- Constructor (Carga el .inf) ---
Lexico::Lexico() {
    std::ifstream archivo("Gramatica/compilador.inf");
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir 'Gramatica/compilador.inf'" << endl; system("PAUSE"); return;
    }
    std::string palabra;
    int id;
    while (archivo >> palabra >> id) {
        tablaSimbolos[palabra] = id;
    }
    archivo.close();
    cout << "Constructor de Lexico: ¡TABLA DE SIMBOLOS CARGADA!" << endl;
    cout << "Se cargaron " << tablaSimbolos.size() << " simbolos." << endl;
    cout << "****************************************" << endl;
    ind = 0;
    continua = false;
    buffer = ""; // Inicializa el buffer
}

// --- Constructor alternativo ---
Lexico::Lexico(string fuente) : Lexico() {
    entrada(fuente);
}

// --- Método para poner el string de entrada ---
void Lexico::entrada(string fuente) {
    this->fuente = fuente + "$";
    this->ind = 0;
    this->continua = true;
}

// *******************************************************
// ---- ¡FUNCIÓN SIGSIMBOLO()
// *******************************************************
int Lexico::sigSimbolo() {
    buffer = ""; // Limpia el buffer al inicio

    if (!continua) {
        buffer = "$";
        return tablaSimbolos["$"];
    }

    // Ignorar espacios en blanco
    while (isspace(fuente[ind])) {
        ind++;
    }

    // 1. Revisa si es una letra (inicio de palabra)
    if (isalpha(fuente[ind])) {
        while (isalnum(fuente[ind])) {
            buffer += fuente[ind]; // Guarda la palabra
            ind++;
        }
    }
    // 2. Revisa si es un símbolo (o el fin)
    else if (fuente[ind] == '$') {
        buffer = "$";
        continua = false;
        ind++;
    }
    else if (fuente[ind] == ';') {
        buffer = ";";
        ind++;
    }
    // (Aquí puedes añadir más 'else if' para '(', ')', '{', '}', etc.)

    // 3. Si no es nada de eso, es un error
    else {
        cout << "LEXICO: Error, simbolo no reconocido: " << fuente[ind] << endl;
        continua = false;
        return -1;
    }

    // --- BÚSQUEDA EN EL MAPA ---

    // 4. ¿La palabra es un "tipo" especial?
    if (buffer == "int" || buffer == "float" || buffer == "char") {
        cout << "LEXICO: Envio '" << buffer << "' como 'tipo' (4)" << endl;
        tipo = tablaSimbolos["tipo"];
        simbolo = buffer[0];
        return tipo;
    }

    // 5. ¿Es otra palabra clave (como "if", ";", "$")?
    if (tablaSimbolos.count(buffer)) {
        cout << "LEXICO: Envio '" << buffer << "' (" << tablaSimbolos[buffer] << ")" << endl;
        tipo = tablaSimbolos[buffer];
        simbolo = buffer[0];
        return tipo;
    }

    // 6. Si no, es un "identificador"
    cout << "LEXICO: Envio '" << buffer << "' como 'identificador' (0)" << endl;
    tipo = tablaSimbolos["identificador"];
    simbolo = buffer[0];
    return tipo;
}

// --- NUEVA FUNCIÓN ---
string Lexico::getBuffer(){
    return buffer;
}

// --- Método terminado() ---
bool Lexico::terminado() {
    return !continua;
}
