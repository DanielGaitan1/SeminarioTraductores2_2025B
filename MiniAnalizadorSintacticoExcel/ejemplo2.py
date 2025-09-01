from lexico import Lexico

def ejemplo2():
    lexico = Lexico("+-+")
    while not lexico.terminado():
        tok = lexico.sigSimbolo()
        print(tok)

if __name__ == "__main__":
    ejemplo2()
