package factory.factory;

import factory.factory.details.Detail;

import java.util.LinkedList;
import java.util.Queue;

public class DetailStorage<T extends Detail> {
    private final int capacity;
    private final Queue<T> storage;

    public DetailStorage(int capacity) {
        this.capacity = capacity;
        this.storage = new LinkedList<>();
    }

    public synchronized void add(T part) throws InterruptedException {
        while (storage.size() >= capacity) {
            wait();
        }
        storage.add(part);
        notifyAll();
    }

    public synchronized T get() throws InterruptedException {
        while (storage.isEmpty()) {
            wait();
        }
        T part = storage.poll();
        notifyAll();
        return part;
    }
}
