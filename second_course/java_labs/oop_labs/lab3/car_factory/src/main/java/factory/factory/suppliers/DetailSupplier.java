package factory.factory.suppliers;

import factory.factory.details.Detail;
import factory.factory.DetailStorage;
import factory.utils.Statistics;

public abstract class DetailSupplier<T extends Detail> implements Runnable {
    private final Statistics statistics;
    private final DetailStorage<T> storage;
    private volatile long productionTime;
    private volatile boolean isRunning;

    public DetailSupplier(Statistics statistics, DetailStorage<T> storage, long productionTime) {
        this.statistics = statistics;
        this.storage = storage;
        this.productionTime = productionTime;
        this.isRunning = true;
    }

    protected abstract T createCarPart(long id);

    public void updateProductionTime(long newProductionTime) {
        this.productionTime = newProductionTime;
    }

    @Override
    public void run() {
        long id = 1;
        while (isRunning) {
            try {
                T part = createCarPart(id++);
                storage.add(part);
                statistics.incrementSuppliedParts(part.getClass());
                Thread.sleep(productionTime);
            } catch (InterruptedException e) {
                isRunning = false;
            }
        }
    }

    public void stop() {
        isRunning = false;
    }
}
