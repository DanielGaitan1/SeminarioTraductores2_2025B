# pila.py
class Pila:
    def __init__(self):
        self.items = []

    def push(self, x):
        self.items.append(x)

    def pop(self):
        if not self.vacia():
            return self.items.pop()
        return None

    def top(self):
        if not self.vacia():
            return self.items[-1]
        return None

    def vacia(self):
        return len(self.items) == 0

    def muestra(self):
        print("Pila:", " ".join(e.muestra() for e in self.items))
