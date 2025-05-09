import java.net.MalformedURLException;
import java.rmi.registry.LocateRegistry;
import java.rmi.*;
import java.rmi.registry.Registry;
import java.rmi.server.*;


import java.util.Scanner;
import java.util.ArrayList;


public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boolean running = true;
        int option = 0;

        while (running) {
            System.out.println("Seleccione una opción:");
            System.out.println("1. Iniciar sesión");
            System.out.println("2. Salir");
            option = scanner.nextInt();
            scanner.nextLine(); // Limpiamos
            int port = -1;

            switch (option) {
                case 1:
                    System.out.print("Ingrese su nombre: ");
                    String name = scanner.nextLine();

                    // Consultamos si el cliente está registrado
                    // Nos conectamos al primero que consultará al otro también

                    boolean isRegistered = false;

                    // En caso de que si esté registrado ya tiene en port el puerto
                    // del servidor al que está registrado, en caso de que no
                    // ya lo obtendrá cuando se registre
                    try {
                        Registry registryLogin = LocateRegistry.getRegistry("127.0.0.1", 1099);
                        DonationInterface donationProgramLogin = (DonationInterface) registryLogin.lookup("servidor_1099");
                        if ((port=donationProgramLogin.registered(name, new ArrayList<>())) > 0){
                            isRegistered = true;
                        }
                        else if (port == -2){
                            System.out.println("Error al consultar el registro.");
                            System.exit(0);
                        }
                    }
                    catch (NotBoundException | RemoteException e) {
                        System.out.println("Error al conectar con el servidor: " + e.getMessage());
                    }

                    if (!isRegistered) {
                        System.out.println("No está registrado. Seleccione una opción:");
                        System.out.println("1. Registrarse");
                        System.out.println("2. Salir");
                        option = scanner.nextInt();
                        scanner.nextLine(); // Limpiamos

                        if (option == 1) {
                            System.out.print("Ingrese un nombre para registrarse: ");
                            String newName = scanner.nextLine();

                            // Registramos el usuario conectando al servidor inicial
                            // El servidor comprobará si está registrado o no por seguridad
                            // y devolverá el puerto correspondiente
                            try {
                                Registry registryLogin = LocateRegistry.getRegistry("127.0.0.1", 1099);
                                DonationInterface donationProgramLogin = (DonationInterface) registryLogin.lookup("servidor_1099");
                                port=donationProgramLogin.register(name);
                            }
                            catch (NotBoundException | RemoteException e) {
                                System.out.println("Error al conectar con el servidor: " + e.getMessage());
                            }

                            System.out.println("Usuario " + newName + " registrado con éxito.");
                            isRegistered = true; // Cliente registrado ahora puede donar
                        } else if (option == 2) {
                            break;
                        }
                    }

                    if (isRegistered) {
                        boolean sessionActive = true;
                        System.out.println("Bienvenido, " + name + "!");

                        // Conectamos con el servidor que le corresponde
                        DonationInterface donationProgram = null;
                        try {
                            Registry registry = LocateRegistry.getRegistry("127.0.0.1", port);
                            donationProgram = (DonationInterface) registry.lookup("servidor_"+port);
                        }
                        catch (NotBoundException | RemoteException e) {
                            System.out.println("Error al conectar con el servidor: " + e.getMessage());
                        }

                        while (sessionActive) {
                            System.out.println("Operaciones disponibles:");
                            System.out.println("1. Donar");
                            System.out.println("2. Total donado");
                            System.out.println("3. Benefactores");
                            System.out.println("4. Cerrar sesión");
                            System.out.println("5. Salir");
                            option = scanner.nextInt();
                            scanner.nextLine(); // Limpiar buffer

                            switch (option) {
                                case 1:
                                    System.out.println("Ejecutando Donar...");
                                    System.out.print("Ingrese la cantidad a donar: ");
                                    float amount = scanner.nextFloat();
                                    scanner.nextLine(); // Limpiamos
                                    try {
                                        donationProgram.donate(name, amount);
                                        System.out.println("Donación de " + amount + " realizada con éxito.");
                                    } catch (Exception e) {
                                        System.out.println("Error al realizar la donación: " + e.getMessage());
                                    }
                                    break;
                                case 2:
                                    System.out.println("Ejecutando getTotal...");
                                    try {
                                        float total = donationProgram.totalAmountDonated(name, false);
                                        System.out.println("Total donado: " + total);
                                    } catch (Exception e) {
                                        System.out.println("Error al obtener el total donado: " + e.getMessage());
                                    }
                                    break;
                                case 3:
                                    System.out.println("Ejecutando getBenefactors...");
                                    try {
                                        ArrayList<String> benefactors = donationProgram.benefactors(name, false);
                                        System.out.println("Benefactores:");
                                        for (String client : benefactors) {
                                            System.out.println(client);
                                        }
                                    } catch (Exception e) {
                                        System.out.println("Error al obtener los benefactores: " + e.getMessage());
                                    }
                                    break;
                                case 4:
                                    sessionActive = false;
                                    break;
                                case 5:
                                    sessionActive = false;
                                    running = false;
                                    break;
                                default:
                                    System.out.println("Opción no válida.");
                            }
                        }
                    }
                    break;
                case 2:
                    running = false;
                    break;
                default:
                    System.out.println("Opción no válida.");
            }
        }

        scanner.close();
        System.out.println("Programa finalizado.");
        System.exit(0);
    }
}
