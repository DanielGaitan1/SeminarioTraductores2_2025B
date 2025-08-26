# elemento_pila.py
class ElementoPila:
    def muestra(self):
        return "?"

class Estado(ElementoPila):
    def __init__(self, numero):
        self.numero = numero
    def muestra(self):
        return f"[{self.numero}]"

class Terminal(ElementoPila):
    def __init__(self, tipo, lexema):
        self.tipo = tipo
        self.lexema = lexema
    def muestra(self):
        return self.lexema

class NoTerminal(ElementoPila):
    def __init__(self, nombre):
        self.nombre = nombre
    def muestra(self):
        return self.nombre
