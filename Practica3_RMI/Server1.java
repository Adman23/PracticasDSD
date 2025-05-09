// Author: Adam Navarro Megías


import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.net.MalformedURLException;

public class Server1 {
    public static void main(String[] args){
        try {
            // Declaramos lo necesario
            int port = 1099;
            int portReplica = 1100;
            String nameServer = "servidor_"+port;
            String nameServerReplica = "servidor_"+portReplica;
            
            Registry reg = LocateRegistry.createRegistry(port);

            DonationProgram donator = new DonationProgram(nameServer, nameServerReplica);
            
            Naming.rebind("rmi://localhost:1099/"+nameServer, donator);

            System.out.println("Se han encendido el servidor en puerto: "
                                + port);
        }
        catch (RemoteException | MalformedURLException e){
            System.out.println("Exception: " + e.getMessage());
        }
    }
}
