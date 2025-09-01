# ejemplo3.py
from pila import Pila
from lexico import Lexico
import tokens

def ejemplo3():
    # Tabla LR mínima del ejemplo (E -> id)
    # Columnas: 0=id, 1=$, 2=E
    tablaLR = [
        [2, 0, 1],   # estado 0
        [0, -1, 0],  # estado 1  (-1 = aceptación)
        [0, -2, 0],  # estado 2  (-2 = reduce regla 1: E -> id)
    ]

    def columna_de_token(tipo):
        # mapea tus códigos de tokens.py a columnas de la tabla pequeña
        if tipo == tokens.IDENT: return 0   # id
        if tipo == tokens.PESOS: return 1   # $
        return None  # cualquier otro es error para este ejemplo

    pila = Pila()
    pila.push(tokens.PESOS)  # $
    pila.push(0)             # estado 0

    L = Lexico("a")          # entrada: a
    tok = L.sigSimbolo()     # token actual (Token)

    aceptacion = False
    while True:
        fila = pila.top()                  # estado en tope
        col = columna_de_token(tok.tipo)   # columna segun token actual
        if col is None:
            print("Error: token no válido para esta tabla:", tok)
            break

        accion = tablaLR[fila][col]

        pila.muestra()
        print("entrada:", tok.lexema, "| accion:", accion)

        if accion > 0:
            # SHIFT: meter token y estado destino
            pila.push(tok.tipo)    # id
            pila.push(accion)      # estado (2)
            tok = L.sigSimbolo()   # pedir siguiente token
        elif accion < 0:
            if accion == -1:
                print("Aceptación")
                aceptacion = True
                break
            elif accion == -2:
                # REDUCE por regla 1: E -> id (longitud RHS = 1 token + 1 estado = 2 pops)
                pila.pop()  # pop estado
                pila.pop()  # pop id
                # goto con NoTerminal E (columna 2)
                fila = pila.top()
                goto = tablaLR[fila][2]  # columna de E
                # push E y estado goto
                pila.push(2)  # representamos E como '2' en esta tabla chica
                pila.push(goto)
            else:
                print("Reducción desconocida:", accion)
                break
        else:
            print("Error de análisis (celda vacía)")
            break

if __name__ == "__main__":
    ejemplo3()
