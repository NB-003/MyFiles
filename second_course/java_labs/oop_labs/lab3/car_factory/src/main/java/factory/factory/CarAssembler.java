package factory.factory;

import factory.utils.Statistics;
import factory.factory.details.Accessory;
import factory.factory.details.Body;
import factory.factory.details.Engine;

public class CarAssembler implements Runnable {
    private final Statistics statistics;
    private final DetailStorage<Body> bodyStorage;
    private final DetailStorage<Engine> engineStorage;
    private final DetailStorage<Accessory> accessoryStorage;
    private final CarStorage carStorage;
    private volatile long assemblyTime;
    private volatile boolean isRunning;

    public CarAssembler(Statistics statistics, DetailStorage<Body> bodyStorage, DetailStorage<Engine> engineStorage,
                        DetailStorage<Accessory> accessoryStorage, CarStorage carStorage, long assemblyTime) {
        this.statistics = statistics;
        this.bodyStorage = bodyStorage;
        this.engineStorage = engineStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
        this.assemblyTime = assemblyTime;
        this.isRunning = true;
    }

    public void updateAssemblyTime(long newAssemblyTime) {
        this.assemblyTime = newAssemblyTime;
    }

    @Override
    public void run() {
        long carId = 1;
        while (isRunning) {
            try {
                Body body = bodyStorage.get();
                Engine engine = engineStorage.get();
                Accessory accessory = accessoryStorage.get();
                Car car = new Car(carId++, body, engine, accessory);
                carStorage.add(car);
                statistics.incrementCarsAssembled();
                Thread.sleep(assemblyTime);
            } catch (InterruptedException e) {
                isRunning = false;
            }
        }
    }

    public void stop() {
        isRunning = false;
    }
}
