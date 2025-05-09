import java.rmi.registry.LocateRegistry;
import java.rmi.*;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.net.MalformedURLException;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class DonationProgram extends UnicastRemoteObject implements DonationInterface{
    
    private String serverName = "";
    private String replicaName = "";
    private int port = 0;
    private int replicaPort = 0;
    private Map<String, Client> registeredClients = new HashMap<>();
    private float totalLocalDonations = 0;
    

    /*
     * El constructor lo que hará es comprobar que servidor se tiene
     * que asignar y que servidor será su vecino, para hacerlo utiliza el 
     * booleano que se manda desde el servidor al construirlo
     */
    DonationProgram(String serverName1, String serverName2, boolean firstOne)throws RemoteException {
        if (firstOne){
            this.serverName = serverName1;
            this.replicaName = serverName2;
        }
        else{
            this.serverName = serverName2;
            this.replicaName = serverName1;
        }
        this.port = Integer.parseInt(this.serverName.split("_")[1]);
        this.replicaPort = Integer.parseInt(this.replicaName.split("_")[1]);
    }

    @Override
    public int registered(String clientName)throws RemoteException {
        if (registeredClients.containsKey(clientName)){
            return this.port;
        }
        else{
            try{
                Registry registry = LocateRegistry.getRegistry("127.0.0.1", this.replicaPort);
                DonationInterface donationProgram = (DonationInterface) registry.lookup(this.replicaName);
                if (donationProgram.registered(clientName) != -1){ 
                    System.out.println("El cliente: " + clientName + " está registrado en la replica");
                    return this.replicaPort; // Si esta registrado en la replica
                }
                else {
                    return -1;  // Si no está registrado en ninguno
                }
            }
            catch (NotBoundException | RemoteException e) {
                System.out.println("Error al conectar con el servidor replica:");
                System.out.println("Server Name: " + this.serverName + ", Replica Name: " + this.replicaName);
                System.out.println("Replica Port: " + this.replicaPort);
                e.printStackTrace(); // Print full stack trace for debugging
            }
        }
        return -1; // Si no está registrado en ninguno
    }

    @Override
    public int register(String clientName)throws RemoteException {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public void donate(String clientName, float amount)throws RemoteException {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public float totalAmountDonated(String clientName)throws RemoteException {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    @Override
    public ArrayList<Client> benefactors(String clientName)throws RemoteException {
        throw new UnsupportedOperationException("Not supported yet.");
    }
}
