// Imports RMI
import java.rmi.Remote;
import java.rmi.RemoteException;

// Import de Utilidad
import java.util.ArrayList;

public interface DonationInterface  extends Remote{
    
    // Funciones de registro de clientes, la primera es para 
    // controlar las opciones del cliente, la segunda para registrarlo
    // en caso de que se lanzase register y ya estuviera registrado
    // (no debería ya que se controla desde el cliente) no se haría nada.

    // En caso de que esté registrado devuelve el nombre del servidor (que tiene el puerto)
    // Cuando lo registra le devuelve el puerto también
    
    public int registered(String clientName, ArrayList<String> visitedServers) throws RemoteException;
    public int register(String clientName) throws RemoteException;

    // La restricción es que esté registrado
    public void donate(String clientName, float amount) throws RemoteException;
    
    // Como la restricción es que haya donado una vez se comprueba ese valor
    public float totalAmountDonated(String clientName, boolean onlyGetValue) throws RemoteException;

    // Devolverá tal cual el listado de clientes que hayan donado,
    // esto se puede Shacer teniendo 2 arrayList(tedioso)
    // o se puede hacer filtrando el arrayList que se tenga de clientes (mas práctico)
    public ArrayList<String> benefactors(String clientName, boolean onlyGetValue) throws RemoteException;

    public int getNumClients() throws RemoteException;
}
