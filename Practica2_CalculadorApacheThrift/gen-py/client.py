import math
from tkinter import ttk

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
        'V', '^', 'C', 'back',
        '1', '2', '3', '/',
        '4', '5', '6', '*',
        '7', '8', '9', '-',
        '0', '.', '!', '+',
        '%', 'mcm', 'mcd', '=',
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
        # Creamos las caracteristicas iniciales de nuestra ventana
        self.root = tk.Tk()
        self.root.geometry('800x800')
        self.max_text_length = 50

        # Creamos dos pestañas, una tendrá una calculadora más básica y otra tendrá
        # Operaciones como vectores o mcm y mcd
        self.notebook = ttk.Notebook(self.root)
        self.tab1 = ttk.Frame(self.notebook)
        self.tab2 = ttk.Frame(self.notebook)

        self.notebook.add(self.tab1, text='Basic')
        self.notebook.add(self.tab2, text='Advanced')
        self.notebook.pack(expand=1, fill='both')

        self.notebook.select(self.tab1)

        # Añadimos los componentes de la pestaña 1
        tk.Label(self.tab1, text="Calculator", font=("Arial", 20)).pack(padx=10, pady=10)

        self.result = tk.StringVar()
        self.result.trace_add("write", self.limit_tam)
        self.entry = tk.Entry(self.tab1, state="readonly" , textvariable=self.result, font=("Arial", 12), bd=10 , width=self.max_text_length, borderwidth=4)
        self.entry.pack(padx=10, pady=10)


        self.buttonframe = tk.Frame(self.tab1)
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
            if button == "V" or button == "mcm" or button == "mcd":
                tk.Button(self.buttonframe, text=button, command=self.changeFrame).grid(row=row, column=column,
                                                                                            sticky=tk.W+tk.E+tk.S+tk.N)
            else:
                (tk.Button(self.buttonframe, text=button, command=lambda b=button: self.parse_button(b))
                    .grid(row=row, column=column, sticky=tk.W+tk.E+tk.S+tk.N))

            if column < 3:
                column+=1
            else:
                row+=1
                column = 0

        # Una vez añadido todos los elementos de la primera pestaña añadimos los de la segunda
        # Minimo comun multiplo
        self.mcm_mcd_fram = ttk.Frame(self.tab2)

        tk.Label(self.mcm_mcd_fram, text="MCM: ", font=("Arial", 20)).grid(row=0, column=0, padx=10, pady=10)

        self.mcm_entries = []
        for i in range(4):
            entry = tk.Entry(self.mcm_mcd_fram, font=("Arial", 13), width=5)
            entry.grid(row=0, column=i+1, padx=5, pady=5)
            self.mcm_entries.append(entry)

        self.mcm_result = tk.StringVar()
        mcm_result_label = tk.Label(self.mcm_mcd_fram, textvariable=self.mcm_result)
        mcm_result_label.grid(row=0, column=6, padx=5, pady=5)

        self.mcm_result.set("result")

        calc_button = tk.Button(self.mcm_mcd_fram, text="Calculate", command=self.calculate_mcm)
        calc_button.grid(row=1, column=0, padx=5)

        # Maximo comun divisor

        tk.Label(self.mcm_mcd_fram, text="MCD: ", font=("Arial", 20)).grid(row=2, column=0, padx=10, pady=10)

        self.mcd_entries = []
        for i in range(4):
            entry = tk.Entry(self.mcm_mcd_fram, font=("Arial", 13), width=5)
            entry.grid(row=2, column=i+1, padx=5, pady=5)
            self.mcd_entries.append(entry)

        self.mcd_result = tk.StringVar()
        mcd_result_label = tk.Label(self.mcm_mcd_fram, textvariable=self.mcd_result)
        mcd_result_label.grid(row=2, column=6, padx=5, pady=5)

        self.mcd_result.set("result")

        calc_button = tk.Button(self.mcm_mcd_fram, text="Calculate", command=self.calculate_mcd)
        calc_button.grid(row=3, column=0, padx=5)

        self.mcm_mcd_fram.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)


    def limit_tam(self, name, index, mode):
        max_length = self.max_text_length-1
        text = self.result.get()
        if len(text) > max_length:
            self.result.set(text[:max_length+1])

    def show(self):
        self.root.mainloop()

    def changeFrame(self):
        self.notebook.select(self.tab2)

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

    def calculate_mcm(self):
        nums = []
        for entry in self.mcm_entries:
            text = entry.get()
            nums.append(int(text))

        self.mcm_result.set(client.mcm(nums))

    def calculate_mcd(self):
        nums = []
        for entry in self.mcd_entries:
            text = entry.get()
            nums.append(int(text))

        self.mcd_result.set(client.mcd(nums))

    def calculate(self):
        text = self.result.get()
        text.strip()
        tokens = text.split() # Tokenizamos la entrada y la recorremos
        if len(tokens) < 2: return

        res = 0


        if tokens[1] == "!":
            res = client.factorial(int(float(tokens[0])))
            tokens.remove(tokens[0])
            tokens.remove(tokens[0])
        elif tokens[1] in '^+-*/%' and len(tokens) >= 3:
            if tokens[1] == '%':
                res = self.operations[tokens[1]](int(tokens[0]), int(tokens[2]))
            else:
                res = self.operations[tokens[1]](float(tokens[0]), float(tokens[2]))
            tokens.remove(tokens[0])
            tokens.remove(tokens[0])
            tokens.remove(tokens[0])
        else:
            res = "Operacion mal construida"


        for indice, token in enumerate(tokens):  # Parseamos y vamos llamando a las funciones pertinentes
            try:
                if token in '^!+-*/%':
                    if token == "!":
                        res = client.factorial(int(float(res)))
                        tokens.remove(token)
                    else:
                        if token == '%':
                            res = self.operations[token](int(res), int(tokens[indice + 1]))
                        else:
                            res = self.operations[token](float(res), float(tokens[indice + 1]))

                        tokens.remove(tokens[indice + 1])
                        tokens.remove(token)
                else:
                    raise ValueError

            except ValueError:
                print("Valor incorrecto introducido")


        self.result.set(str(res))


if __name__ == '__main__':
    calculator = Calculator()
    calculator.show()


####################################################################
transport.close()
