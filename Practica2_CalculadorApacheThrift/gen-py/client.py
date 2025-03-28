from calculator import CalculatorService

from thrift import Thrift
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

transport = TSocket.TSocket("localhost", 9090)
transport = TTransport.TBufferedTransport(transport)
protocol = TBinaryProtocol.TBinaryProtocol(transport)

client = CalculatorService.Client(protocol)

transport.open()
# Entre el open y el close es la lógica del cliente, que hará la entrada y las llamadas a las funciones
####################################################################
dic = {"SUMA":0,"MULTIPLICACA":1,"DIVISION":2}
operacion = str(input())
if not dic[operacion]:
    exit(1)





print("hacemos ping al server")
client.ping()

resultado = client.suma(1, 1)
print("1 + 1 = " + str(resultado))
resultado = client.resta(1, 1)
print("1 - 1 = " + str(resultado))

####################################################################
transport.close()
