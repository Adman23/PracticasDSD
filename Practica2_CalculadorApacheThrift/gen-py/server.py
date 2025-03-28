import glob
import sys
from argparse import ArgumentError

from calculator import CalculatorService

from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

import logging

logging.basicConfig(level=logging.DEBUG)


# Aquí se implementa la funcionalidad definida en el .thrift, lo otro es similar en los .server
####################################################################
class EmptyListError(Exception):
    pass

# Decorador de python para manejar excepciones
def handle_list_exceptions(operation):
    def wrapper(self, *args): # Encapsula las operaciones para añadirle la funcionalidad
        try:
            # Si tiene una lista como primer argumento
            if args and isinstance(args[0], list) and len(args[0]) == 1:
                nums = args[0]
                if nums is None or len(nums) == 0: # Hace esta verificación
                    raise EmptyListError("The list passed by argument is empty")
                if (all(isinstance(num, float) for num in nums) or
                    all(isinstance(num, int) for num in nums)):
                    raise ValueError("The passed arguments are incorrect")

            return operation(self, *args) # Si es correcto ejecuta la función normal

        except EmptyListError as e:
            print(e) # En caso de error se muestra
            return e
        except ValueError as e:
            print(e) # En caso de error se muestra
            return e
    return wrapper


# Decorador de python para manejar excepciones
def handle_two_floats_exception(operation):
    def wrapper(self, *args): # Encapsula las operaciones para añadirle la funcionalidad
        try:

            if not isinstance(args[0], float) and not isinstance(args[1],float):  # Comprobamos que el segundo argumento también
                raise ValueError("The passed arguments are incorrect")
            elif operation.__name__ == "div" and args[1] == 0:
                raise ZeroDivisionError("The list passed by argument is empty")

            return operation(self, *args) # Si es correcto ejecuta la función normal

        except ValueError as e:
            print(e) # En caso de error se muestra
            return e
        except ZeroDivisionError as e:
            print(e) # En caso de error se muestra
            return e
    return wrapper



class CalculatorHandler:
    def __init__(self):
        self.log = {}

    @handle_list_exceptions
    def mul(self, nums):
        result = 1

        for num in nums:
            result *= num

        return result

    @handle_list_exceptions
    def sum(self, nums):
        result = 0

        for num in nums:
            result += num

        return result

    @handle_two_floats_exception
    def sub(self, n1, n2):
        return n1 - n2

    @handle_two_floats_exception
    def div(self, n1, n2):
        return n1 / n2

    @handle_list_exceptions
    def mcd(self, nums):
        return "Not implemented"

    @handle_list_exceptions
    def mcm(self, nums):
        return "Not implemented"

    def factorial(self, num):
        return "Not implemented"


####################################################################
if __name__ == "__main__":
    handler = CalculatorHandler()
    processor = CalculatorService.Processor(handler)
    transport = TSocket.TServerSocket(host="127.0.0.1", port=9090)
    tfactory = TTransport.TBufferedTransportFactory()
    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)

    print("iniciando servidor...")
    server.serve()
    print("fin")
