package factory.factory;

import java.util.LinkedList;
import java.util.Queue;

public class CarStorage {
    private final int capacity;
    private final Queue<Car> storage;

    public CarStorage(int capacity) {
        this.capacity = capacity;
        this.storage = new LinkedList<>();
    }

    public synchronized void add(Car car) throws InterruptedException {
        while (storage.size() >= capacity) {
            wait();
        }
        storage.add(car);
        notifyAll();
    }

    public synchronized Car get() throws InterruptedException {
        while (storage.isEmpty()) {
            wait();
        }
        Car car = storage.poll();
        notifyAll();
        return car;
    }
}
