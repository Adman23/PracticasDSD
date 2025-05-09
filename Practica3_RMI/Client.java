import  java.util.ArrayList;

public class Client {
    private String name;
    private float totalDonated;
    private ArrayList<Float> donations;

    Client(String name){
        this.name = name;
        this.totalDonated = 0;
        this.donations = new ArrayList<>();
    }

    // Getters y Setters
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public float getTotalDonated() {
        return totalDonated;
    }
    public ArrayList<Float> getDonations() {
        return donations;
    }
    

    // Función de donar
    public void donate(float amount){
        this.donations.add(amount);
        this.totalDonated += amount;
    }
    
}
