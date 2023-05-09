package factory.threadpool;

import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.AbstractExecutorService;
import java.util.concurrent.TimeUnit;

public class ThreadPool extends AbstractExecutorService {
    private final Queue<Runnable> tasks;
    private final List<Worker> workers;
    private volatile boolean isShutdown = false;

    public ThreadPool(int workerCount) {
        this.tasks = new LinkedList<>();
        this.workers = new LinkedList<>();

        for (int i = 0; i < workerCount; i++) {
            Worker worker = new Worker();
            worker.setDaemon(true);
            worker.start();
            workers.add(worker);
        }
    }

    @Override
    public void execute(Runnable command) {
        synchronized (tasks) {
            if (!isShutdown) {
                tasks.add(command);
                tasks.notify();
            }
        }
    }

    @Override
    public void shutdown() {
        isShutdown = true;
        synchronized (tasks) {
            tasks.notifyAll();
        }
    }

    @Override
    public List<Runnable> shutdownNow() {
        List<Runnable> remainingTasks;
        synchronized (tasks) {
            isShutdown = true;
            remainingTasks = new LinkedList<>(tasks);
            tasks.clear();
            tasks.notifyAll();
        }
        return remainingTasks;
    }

    @Override
    public boolean isShutdown() {
        return isShutdown;
    }

    @Override
    public boolean isTerminated() {
        if (!isShutdown) {
            return false;
        }
        for (Worker worker : workers) {
            if (worker.isAlive()) {
                return false;
            }
        }
        return true;
    }

    @Override
    public boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
        long remainingNanos = unit.toNanos(timeout);
        long deadline = System.nanoTime() + remainingNanos;
        for (Worker worker : workers) {
            if (remainingNanos <= 0) {
                return false;
            }
            worker.join(TimeUnit.NANOSECONDS.toMillis(remainingNanos));
            remainingNanos = deadline - System.nanoTime();
        }
        return true;
    }

    private class Worker extends Thread {
        @Override
        public void run() {
            while (!isShutdown || !tasks.isEmpty()) {
                Runnable task;
                synchronized (tasks) {
                    while (tasks.isEmpty()) {
                        if (isShutdown) {
                            return;
                        }
                        try {
                            tasks.wait();
                        } catch (InterruptedException e) {
                            return;
                        }
                    }
                    task = tasks.poll();
                }
                task.run();
            }
        }
    }
}
