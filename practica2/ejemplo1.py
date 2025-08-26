from pila import Pila

def ejemplo1():
    pila = Pila()
    pila.push(2)
    pila.push(3)
    pila.push(4)
    pila.push(5)
    pila.muestra()

    print("tope:", pila.top())
    print("tope:", pila.top())

    print("pop:", pila.pop())
    print("pop:", pila.pop())
    pila.muestra()

if __name__ == "__main__":
    ejemplo1()
