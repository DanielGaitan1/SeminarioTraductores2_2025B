# pruebas.py
from lexico import Lexico

ejemplo = "int x = 10; float pi=3.14; if(x>=10) return x+1;"

L = Lexico(ejemplo)

while True:
    tok = L.sigSimbolo()
    print(tok)
    if tok.tipo == 23:  # PESOS
        break
