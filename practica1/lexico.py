# lexico.py
import string
import tokens

class Token:
    def __init__(self, tipo, lexema):
        self.tipo = tipo
        self.lexema = lexema
    def __repr__(self):
        return f"<{self.tipo}:{self.lexema}>"

class Lexico:
    def __init__(self, cadena):
        self.cadena = cadena
        self.i = 0

    def terminado(self):
        return self.i >= len(self.cadena)

    def sigSimbolo(self):
        # saltar espacios
        while not self.terminado() and self.cadena[self.i].isspace():
            self.i += 1
        if self.terminado():
            return Token(tokens.PESOS, "$")

        c = self.cadena[self.i]

        # --- Operadores de dos caracteres ---
        if self.i+1 < len(self.cadena):
            dos = self.cadena[self.i:self.i+2]
            if dos in ("==", "!="):
                self.i += 2
                return Token(tokens.OP_IG, dos)
            if dos in ("<=", ">="):
                self.i += 2
                return Token(tokens.OP_REL, dos)
            if dos == "&&":
                self.i += 2
                return Token(tokens.OP_AND, dos)
            if dos == "||":
                self.i += 2
                return Token(tokens.OP_OR, dos)

        # --- Operadores de un caracter ---
        mapa = {
            '+': tokens.OP_SUMA, '-': tokens.OP_SUMA,
            '*': tokens.OP_MUL,  '/': tokens.OP_MUL,
            '<': tokens.OP_REL,  '>': tokens.OP_REL,
            '=': tokens.ASIG,    '!': tokens.OP_NOT,
            '(': tokens.PARI,    ')': tokens.PARD,
            '{': tokens.LLI,     '}': tokens.LLD,
            ',': tokens.COMA,    ';': tokens.PYC,
        }
        if c in mapa:
            self.i += 1
            return Token(mapa[c], c)

        # --- Número (entero o real) ---
        if c.isdigit():
            j = self.i
            while j < len(self.cadena) and self.cadena[j].isdigit():
                j += 1
            if j < len(self.cadena) and self.cadena[j] == '.':
                k = j+1
                while k < len(self.cadena) and self.cadena[k].isdigit():
                    k += 1
                if k > j+1:  # hay dígitos después del punto
                    lex = self.cadena[self.i:k]
                    self.i = k
                    return Token(tokens.REAL, lex)
            lex = self.cadena[self.i:j]
            self.i = j
            return Token(tokens.ENTERO, lex)

        # --- Identificadores y palabras reservadas ---
        if c.isalpha() or c == '_':
            j = self.i
            while j < len(self.cadena) and (self.cadena[j].isalnum() or self.cadena[j]=='_'):
                j += 1
            lex = self.cadena[self.i:j]
            self.i = j
            if lex in ("if",): return Token(tokens.IF, lex)
            if lex in ("while",): return Token(tokens.WHILE, lex)
            if lex in ("return",): return Token(tokens.RETURN, lex)
            if lex in ("else",): return Token(tokens.ELSE, lex)
            if lex in ("int", "float", "void"): return Token(tokens.TIPO, lex)
            return Token(tokens.IDENT, lex)

        # --- Si nada coincide ---
        self.i += 1
        return Token(tokens.PESOS, c)
