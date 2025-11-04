#include "lexico.h"

// --- Constructor (Aquí cargamos el .inf) ---
Lexico::Lexico() {
    // 1. Abrimos el archivo .inf
    std::ifstream archivo("compilador.inf");

    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo abrir 'compilador.inf'" << endl;
        system("PAUSE");
        return;
    }

    // 2. Leemos cada línea para llenar el mapa
    std::string palabra;
    int id;
    while (archivo >> palabra >> id) {
        // Guarda en el mapa, ej: tablaSimbolos["if"] = 19;
        tablaSimbolos[palabra] = id;
    }

    archivo.close();

    cout << "Constructor de Lexico: ¡TABLA DE SIMBOLOS CARGADA!" << endl;
    cout << "Se cargaron " << tablaSimbolos.size() << " simbolos." << endl;
    cout << "****************************************" << endl;

    // Inicializamos variables
    ind = 0;
    continua = false;
}

// --- Constructor alternativo ---
Lexico::Lexico(string fuente) : Lexico() { // Llama al constructor de arriba
    entrada(fuente); // Y luego llama a entrada()
}

// --- Método para poner el string de entrada ---
void Lexico::entrada(string fuente) {
    this->fuente = fuente + "$"; // Agregamos el $ al final
    this->ind = 0;
    this->continua = true;
}


int Lexico::sigSimbolo() {
    if (!continua) {
        return tablaSimbolos["$"]; // Devuelve 23 (ID de $)
    }

    string buffer = "";

    // Ignorar espacios en blanco
    while (isspace(fuente[ind])) {
        ind++;
    }

    // 1. Revisa si es una letra (inicio de palabra)
    if (isalpha(fuente[ind])) {
        // Acumula la palabra completa
        while (isalnum(fuente[ind])) {
            buffer += fuente[ind];
            ind++;
        }
    }
    // 2. Revisa si es un símbolo de 1 caracter (o el fin)
    else if (fuente[ind] == '$') {
        buffer = "$";
        continua = false;
        ind++;
    }
    else if (fuente[ind] == ';') { // <--- AÑADIMOS PUNTO Y COMA
        buffer = ";";
        ind++;
    }
    // 3. Si no es nada de eso, es un error
    else {
        cout << "LEXICO: Error, simbolo no reconocido: " << fuente[ind] << endl;
        continua = false;
        return -1;
    }

    // --- BÚSQUEDA EN EL MAPA (LÓGICA MEJORADA) ---

    // 4. ¿La palabra es un "tipo" especial?
    if (buffer == "int" || buffer == "float" || buffer == "char") { // (puedes agregar más)
        cout << "LEXICO: Envio '" << buffer << "' como 'tipo' (4)" << endl;
        tipo = tablaSimbolos["tipo"]; // Devuelve 4
        simbolo = buffer[0];
        return tipo;
    }

    // 5. ¿Es otra palabra clave (como "if", "while")?
    if (tablaSimbolos.count(buffer)) {
        // ¡Sí! Es "if", "while", ";", "$", etc.
        cout << "LEXICO: Envio '" << buffer << "' (" << tablaSimbolos[buffer] << ")" << endl;
        tipo = tablaSimbolos[buffer];
        simbolo = buffer[0];
        return tipo;
    }

    // 6. Si no es nada de lo anterior, AHORA SÍ es un "identificador"
    cout << "LEXICO: Envio '" << buffer << "' como 'identificador' (0)" << endl;
    tipo = tablaSimbolos["identificador"];
    simbolo = buffer[0];
    return tipo;
}

// --- Método terminado() ---
bool Lexico::terminado() {
    return !continua;
}
