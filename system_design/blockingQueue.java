import java.util.LinkedList;
import java.util.Queue;

public class BlockingQueue {
    private Queue<Integer> queue = new LinkedList<Integer>();
    private int limit = 10;
    
    public BlockingQueue(int limit){
        this.limit = limit;
    }
    
    public synchronized void enqueue(Integer item) throws InterruptedException{
        while(queue.size()==limit)
            wait();
        if(queue.size()==0)
            notifyAll();
        queue.add(item);
    }
    
    public synchronized int dequeue() throws InterruptedException{
        while(queue.size()==0)
            wait();
        if(queue.size()==limit)
            notifyAll();
        return queue.remove();
    }
}
