import math

from calculator import CalculatorService

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

import tkinter as tk

transport = TSocket.TSocket("localhost", 9090)
transport = TTransport.TBufferedTransport(transport)
protocol = TBinaryProtocol.TBinaryProtocol(transport)

client = CalculatorService.Client(protocol)

transport.open()
# Entre el open y el close es la lógica del cliente, que hará la entrada y las llamadas a las funciones
####################################################################

#Utilizamos tkinter para hacer una pequeña interfaz de usuario que nos servirá para hacer el input

class Calculator:
    buttons = [
        '', '^', 'C', 'back',
        '1', '2', '3', '/',
        '4', '5', '6', '*',
        '7', '8', '9', '-',
        '0', '.', '!', '+',
        '%', '', '', '=',
    ]

    operations = {
        "+": client.sum,
        "-": client.sub,
        "*": client.mul,
        "/": client.div,
        "^": client.pow,
        "%": client.mod,
        "!": client.factorial,
    }

    def __init__(self):
        self.root = tk.Tk()
        self.root.geometry('500x400')
        self.max_text_length = 50

        tk.Label(self.root, text="Calculator", font=("Arial", 20)).pack(padx=10, pady=10)

        self.result = tk.StringVar()
        self.result.trace_add("write", self.limit_tam)
        self.entry = tk.Entry(self.root, state="readonly" , textvariable=self.result, font=("Arial", 12), bd=10 , width=self.max_text_length, borderwidth=4)
        self.entry.pack(padx=10, pady=10)


        self.buttonframe = tk.Frame(self.root)
        self.buttonframe.columnconfigure(0, weight=1)
        self.buttonframe.columnconfigure(1, weight=1)
        self.buttonframe.columnconfigure(2, weight=1)
        self.buttonframe.columnconfigure(3, weight=1)

        self.buttonframe.rowconfigure(0, weight=1)
        self.buttonframe.rowconfigure(1, weight=1)
        self.buttonframe.rowconfigure(2, weight=1)
        self.buttonframe.rowconfigure(3, weight=1)
        self.buttonframe.rowconfigure(4, weight=1)
        self.buttonframe.rowconfigure(5, weight=1)

        self.buttonframe.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        row = 0
        column = 0
        for button in self.buttons:
            (tk.Button(self.buttonframe, text=button, command=lambda b=button: self.parse_button(b))
                 .grid(row=row, column=column, sticky=tk.W+tk.E+tk.S+tk.N))

            if column < 3:
                column+=1
            else:
                row+=1
                column = 0

    def limit_tam(self, name, index, mode):
        max_length = self.max_text_length-1
        text = self.result.get()
        if len(text) > max_length:
            self.result.set(text[:max_length+1])

    def show(self):
        self.root.mainloop()

    def parse_button(self, button):
        if button == '=': self.calculate()
        elif button == 'C': self.clear()
        elif button == 'back': self.go_back()
        elif button in '^!+-*/%':
            self.result.set(self.result.get() + f' {button} ')
        else:
            self.result.set(self.result.get() + button)

    def go_back(self):
        text = self.result.get()
        text.strip()
        partes = text.rsplit(' ', 1)
        if len(partes) > 1:
            new_text = partes[0]
            self.result.set(new_text)
        else:
            self.clear()

    def clear(self):
        self.result.set("")

    def calculate(self):
        text = self.result.get()
        text.strip()
        tokens = text.split() # Tokenizamos la entrada y la recorremos
        if len(tokens) < 2: return

        res = 0

        if tokens[1] == "!":
            res = client.factorial(int(tokens[0]))
            tokens.remove(tokens[0])
            tokens.remove(tokens[1])
        elif tokens[1] in '^+-*/%' and len(tokens) >= 3:
            res = self.operations[tokens[1]](tokens[0], tokens[2])
            tokens.remove(tokens[0])
            tokens.remove(tokens[1])
            tokens.remove(tokens[2])
        else:
            res = "Operacion mal construida"


        for indice, token in tokens:  # Parseamos y vamos llamando a las funciones pertinentes
            try:
                if token in '^!+-*/%':
                    if token == "!":
                        res = client.factorial(res)
                        tokens.remove(token)
                    else:
                        if token == '%':
                            res = self.operations[token](int(res), int(tokens[indice + 1]))
                        else:
                            res = self.operations[token](float(res), float(tokens[indice + 1]))

                        tokens.remove(token)
                        tokens.remove(tokens[indice + 1])
                else:
                    raise ValueError

            except ValueError:
                res = "Valor incorrecto"


        self.result.set(str(res))


if __name__ == '__main__':
    calculator = Calculator()
    calculator.show()


####################################################################
transport.close()
