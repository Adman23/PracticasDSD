

import java.rmi.registry.LocateRegistry;
import java.rmi.*;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.net.MalformedURLException;

public class servidor {
    public static void main(String[] args){
        try {
            // Crea una instancia del contador
            // System.setProperty("java.rmi.server.hostname", 192.168.1.107);
            Registry reg=LocateRegistry.createRegistry(1099);
            contador micontador = new contador();
            Naming.rebind("mmicontador", micontador);
        }
        catch (RemoteException | MalformedURLException e) {
            System.out.println("Exception: " + e.getMessage());
        }
    }
}
