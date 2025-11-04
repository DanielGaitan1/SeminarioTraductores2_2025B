#ifndef _LEXICO
#define _LEXICO

#include <iostream>
#include <string>
#include <map>        // <--- AÑADIDO: Para el "diccionario"
#include <fstream>    // <--- AÑADIDO: Para leer el .inf
#include <sstream>    // <--- AÑADIDO: Para procesar la entrada

using namespace std;

// --- Esta es la clase del profesor (¡Correcta!) ---
class TipoSimbolo{
public:
    static const int ID= 0;
    static const int ENTERO= 50; // (Estos valores son del ejemplo, los reales están en .inf)
    static const int OPSUMA= 51;
    static const int PESOS= 1; // ¡Este es el valor importante! $ = 1
};


class Lexico{
public:
    // --- Miembros del profesor ---
    char simbolo;
    int tipo;

    // --- Constructores y métodos del profesor ---
    Lexico(string fuente);
    Lexico(); // <-- Aquí cargaremos el .inf

    void entrada(string fuente);
    int sigSimbolo(); // <-- Aquí usaremos el "diccionario"
    bool terminado();

private:
    // --- Miembros del profesor ---
    string fuente;
    int ind;

    // --- NUESTRAS ADICIONES ---
    bool continua; // Para controlar el fin
    // "Diccionario" que guarda: {"identificador", 0}, {"if", 19}, {"$", 23}
    std::map<std::string, int> tablaSimbolos;
};

#endif
