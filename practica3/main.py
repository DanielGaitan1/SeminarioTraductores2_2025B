from pila import Pila
from elemento_pila import Estado, Terminal, NoTerminal
from lexico import Lexico
import tokens

def columna_de_token(tipo):
    # columnas de la tabla: 0=id, 1=$, 2=E
    if tipo == tokens.IDENT: return 0
    if tipo == tokens.PESOS: return 1
    return None  # cualquier otro token es error en este ejemplo mínimo

def ejemplo3_objetos():
    # Tabla LR para gramática mínima: E -> id
    # filas = estados; columnas = id, $, E
    tablaLR = [
        [2, 0, 1],   # s0: id -> shift 2; goto(E)=1
        [0, -1, 0],  # s1: aceptar con $
        [0, -2, 0],  # s2: reduce por regla 1 (E -> id)
    ]

    pila = Pila()
    pila.push(Estado(0))           # Estado inicial

    L = Lexico("a")                # Entrada
    tok = L.sigSimbolo()           # Primer token

    while True:
        # El tope DEBE ser un Estado si siempre hacemos push símbolo + Estado
        topo = pila.top()
        if not isinstance(topo, Estado):
            print("Error: tope de pila no es un Estado")
            return

        fila = topo.numero
        col = columna_de_token(tok.tipo)
        if col is None:
            print("Error: token no válido para esta tabla:", tok)
            return

        accion = tablaLR[fila][col]

        pila.muestra()
        print("entrada:", tok.lexema, "| accion:", accion)

        if accion > 0:
            # SHIFT: push Terminal y luego Estado(accion)
            pila.push(Terminal(tok.tipo, tok.lexema))
            pila.push(Estado(accion))
            tok = L.sigSimbolo()

        elif accion < 0:
            if accion == -1:
                print("Aceptación")
                return
            elif accion == -2:
                # REDUCE: E -> id  (longitud RHS = 1 símbolo)
                # En la pila hay ... Estado, Terminal(id), Estado
                pila.pop()  # pop Estado
                pila.pop()  # pop Terminal(id)

                # GOTO por NoTerminal E (columna 2)
                topo = pila.top()
                if not isinstance(topo, Estado):
                    print("Error: tope tras reducción no es Estado")
                    return
                goto = tablaLR[topo.numero][2]  # col de E
                pila.push(NoTerminal("E"))
                pila.push(Estado(goto))
            else:
                print("Reducción desconocida:", accion)
                return
        else:
            print("Error de análisis (celda vacía)")
            return

if __name__ == "__main__":
    ejemplo3_objetos()
