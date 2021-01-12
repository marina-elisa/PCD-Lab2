import java.util.concurrent.Semaphore;

public class TrafficController {

    static Semaphore trafficSemaphore = new Semaphore(1);
    int crossingLR = 0, crossingRL = 0;

    public void enterLeft() {
        if(crossingLR == 0){
            try{
                trafficSemaphore.acquire();
                crossingLR++;
            }catch(InterruptedException e){
                System.out.println("Excecao");
            }
        }else{
            crossingLR++;
        }
        
    }
    public void enterRight() {
        if(crossingRL == 0){
            try{
                trafficSemaphore.acquire();
                crossingRL++;
            }catch(InterruptedException e){
                System.out.println("Excecao");
            }
        }else{
            crossingRL++;
        }
    }
    public void leaveLeft() {
        crossingRL--;
        if(crossingRL == 0) trafficSemaphore.release();
    }
    public void leaveRight() {
        crossingLR--;
        if(crossingLR == 0) trafficSemaphore.release();
    }

}