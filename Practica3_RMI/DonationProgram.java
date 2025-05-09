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
    DonationProgram(String serverName1, String serverName2)throws RemoteException {

        this.serverName = serverName1;
        this.replicaName = serverName2;
        
        this.port = Integer.parseInt(this.serverName.split("_")[1]);
        this.replicaPort = Integer.parseInt(this.replicaName.split("_")[1]);

    }

    @Override
    public int registered(String clientName, ArrayList<String> visitedServers)throws RemoteException {
        if (registeredClients.containsKey(clientName)){
            System.out.println("El cliente: " + clientName + " está registrado en el servidor");
            return this.port;
        }
        
        if (visitedServers.contains(this.serverName)){
            return -1; // Si ya ha visitado este servidor
        }

        try{
            Registry registry = LocateRegistry.getRegistry("127.0.0.1", this.replicaPort);
            DonationInterface donationProgram = (DonationInterface) registry.lookup(this.replicaName);
            visitedServers.add(this.serverName); // Añadimos el servidor visitado
            if (donationProgram.registered(clientName, visitedServers) > 0){ 
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
        
        return -2;
    }

    @Override
    public int register(String clientName)throws RemoteException {
        int portTemp = 0;
        if ((portTemp=this.registered(clientName, new ArrayList<>())) > 0){
            System.out.println("El cliente: " + clientName + " ya está registrado en el puerto: " + portTemp);
            return portTemp;
        }

        // En caso de que el otro servidor tenga menos clientes registrados se le deja a su cargo
        try{
            Registry registry = LocateRegistry.getRegistry("127.0.0.1", this.replicaPort);
            DonationInterface donationProgram = (DonationInterface) registry.lookup(this.replicaName);
            if (this.getNumClients() > donationProgram.getNumClients()){ 
                donationProgram.register(clientName);
                return this.replicaPort; // Si esta registrado en la replica
            }
        }
        catch (NotBoundException | RemoteException e) {
            System.out.println("Error al conectar con el servidor replica:");
            System.out.println("Server Name: " + this.serverName + ", Replica Name: " + this.replicaName);
            System.out.println("Replica Port: " + this.replicaPort);
            e.printStackTrace(); // Print full stack trace for debugging
        }
        
        // En caso de que tengan los mismos clientes o que este servidor tenga menos se registra aquí
        Client client = new Client(clientName);
        registeredClients.put(clientName, client);
        System.out.println("El cliente: " + clientName + " se ha registrado en el servidor: "+ this.serverName);
        
        return this.port;
    }

    @Override
    public int getNumClients()throws RemoteException {
        return registeredClients.size();
    }

    @Override
    public void donate(String clientName, float amount)throws RemoteException {
        if (registeredClients.containsKey(clientName)){
            Client client = registeredClients.get(clientName);
            client.donate(amount);
            totalLocalDonations += amount;
            System.out.println("El cliente: " + clientName + " ha donado: " + amount + "€");
        }
        else {
            System.out.println("El cliente: " + clientName + " no está registrado en el servidor: "+ this.serverName);
        }
    }

    @Override
    public float totalAmountDonated(String clientName, boolean onlyGetValue)throws RemoteException {
        // Si no se necesita que pregunte al otro servidor
        if (onlyGetValue){
            return totalLocalDonations;
        }

        if (registeredClients.containsKey(clientName)){
            Client client = registeredClients.get(clientName);
            if (client.getDonations().isEmpty()){
                System.out.println("El cliente: " + clientName + " no ha donado nada.");
                return 0;
            }
            else {
                float total = totalLocalDonations;

                // Accedemos a los totales de la replica
                try{
                    Registry registry = LocateRegistry.getRegistry("127.0.0.1", this.replicaPort);
                    DonationInterface donationProgram = (DonationInterface) registry.lookup(this.replicaName);
                    total += donationProgram.totalAmountDonated(clientName, true);
                }
                catch (NotBoundException | RemoteException e) {
                    System.out.println("Error al conectar con el servidor replica:");
                    System.out.println("Server Name: " + this.serverName + ", Replica Name: " + this.replicaName);
                    System.out.println("Replica Port: " + this.replicaPort);
                    e.printStackTrace(); // Print full stack trace for debugging
                }
                return total;
            }
        }
        else {
            System.out.println("El cliente: " + clientName + " no está registrado en el servidor: "+ this.serverName);
            return -1;
        }
    }

    @Override
    public ArrayList<String> benefactors(String clientName, boolean onlyGetValue)throws RemoteException {
        // Si no se necesita que pregunte al otro servidor
        if (onlyGetValue){
            ArrayList<String> clientList = new ArrayList<>();
            for (Map.Entry<String, Client> entry : registeredClients.entrySet()) {
                String clientKey = entry.getKey();
                clientList.add(clientKey);
            }
            return clientList;
        }

        if (registeredClients.containsKey(clientName)){
            Client client = registeredClients.get(clientName);
            if (client.getDonations().isEmpty()){
                System.out.println("El cliente: " + clientName + " no ha donado nada.");
                return new ArrayList<>();
            }
            else {
                ArrayList<String> clientList = new ArrayList<>();
                for (Map.Entry<String, Client> entry : registeredClients.entrySet()) {
                    String clientKey = entry.getKey();
                    clientList.add(clientKey);
                }

                // Accedemos a los keys de la replica
                try{
                    Registry registry = LocateRegistry.getRegistry("127.0.0.1", this.replicaPort);
                    DonationInterface donationProgram = (DonationInterface) registry.lookup(this.replicaName);
                    clientList.addAll(donationProgram.benefactors(clientName, true));
                }
                catch (NotBoundException | RemoteException e) {
                    System.out.println("Error al conectar con el servidor replica:");
                    System.out.println("Server Name: " + this.serverName + ", Replica Name: " + this.replicaName);
                    System.out.println("Replica Port: " + this.replicaPort);
                    e.printStackTrace(); // Print full stack trace for debugging
                }
                return clientList;
            }
        }
        else {
            System.out.println("El cliente: " + clientName + " no está registrado en el servidor: "+ this.serverName);
            return new ArrayList<>();
        }        
    }
}
