// Author: Adam Navarro Megías

/*
 * Esta clase lo que va a hacer es inicializar ambas replicas
 * asignando los puertos apropiados, luego creará para cada replica su 
 * objeto correspondiente de ProgramaDonaciones haciendo el bind
 * Para crearlos distintos se pueden asignar los puertos 1099 y 1100 (u otros)
 * y luego crear un nombre compuesto con los mismos para que se identifiquen
 * 
 * o también se pueden asignar 2 nombres y ponerle uno a cada uno
 * 
 * La cosa es que se haga la creación y el bind bien 
 */

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.net.MalformedURLException;

public class Server {
    public static void main(String[] args){
        try {
            // Declaramos lo necesario
            int port1 = 1099;
            int port2 = 1100;
            String nameServer1 = "servidor_"+port1;
            String nameServer2 = "servidor_"+port2;
            
            // Creamos registro en ambos puertos
            Registry reg1 = LocateRegistry.createRegistry(port1);
            Registry reg2 = LocateRegistry.createRegistry(port2);

            // Creamos los objetos que implementan
            DonationProgram donator1 = new DonationProgram(nameServer1, nameServer2, true);
            DonationProgram donator2 = new DonationProgram(nameServer1, nameServer2, false);
            
            Naming.rebind(nameServer1, donator1);
            Naming.rebind(nameServer2, donator2);
            
            //reg1.rebind(nameServer1, donator1);
            //reg2.rebind(nameServer2, donator2);

            System.out.println("Se han encendido los servidores en puertos: "
                                + port1 + " y " + port2);
        }
        catch (RemoteException  | MalformedURLException e){
            System.out.println("Exception: " + e.getMessage());
        }
    }

    
}
