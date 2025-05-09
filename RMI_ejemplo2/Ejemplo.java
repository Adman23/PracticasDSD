import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Ejemplo implements Ejemplo_I {
    public Ejemplo(){
        super();
    }

    public synchronized void escribir_mensaje(String mensaje) {
        System.out.println("\n Entra Hebra " + mensaje);
        
        // Buscamos los procesos 0, 10, 20, ...
        if (mensaje.endsWith("0")) {
            try {
                System.out.println("Empezamos a dormir");
                Thread.sleep(5000);
                System.out.println("Terminamos a dormir");
            }
            catch (Exception e) {
                System.err.println("Ejemplo exception: "+mensaje);
            }
        }
        System.out.println("Sale Hebra " + mensaje);
    }

    public static void main(String args[]) {
        try {
            String nombre_objeto_remoto = "Ejemplo_I";
            Ejemplo_I prueba = new Ejemplo();
            Ejemplo_I stub = (Ejemplo_I) UnicastRemoteObject.exportObject(prueba, 0);
            Registry registry = LocateRegistry.getRegistry();
            registry.rebind(nombre_objeto_remoto, stub);
            System.out.println("Ejemplo bound");
        } catch (Exception e) {
            System.err.println("Ejemplo exception: " );
            e.printStackTrace();
        }
    }

}
