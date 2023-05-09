package factory.factory;

import factory.utils.Logger;
import factory.utils.Statistics;

public class CarDealer implements Runnable {
    private final Statistics statistics;
    private final int dealerNumber;
    private final CarStorage carStorage;
    private volatile long requestTime;
    private volatile boolean isRunning;

    public CarDealer(Statistics statistics, int dealerNumber, CarStorage carStorage, long requestTime) {
        this.statistics = statistics;
        this.dealerNumber = dealerNumber;
        this.carStorage = carStorage;
        this.requestTime = requestTime;
        this.isRunning = true;
    }

    public void updateRequestTime(long newRequestTime) {
        this.requestTime = newRequestTime;
    }

    @Override
    public void run() {
        while (isRunning) {
            try {
                Car car = carStorage.get();
                statistics.incrementCarsSold();
                // Здесь добавляем код для записи информации о продаже в лог-файл
                Logger.logCarSold(car, dealerNumber);
                Thread.sleep(requestTime);
            } catch (InterruptedException e) {
                isRunning = false;
            }
        }
    }

    public void stop() {
        isRunning = false;
    }
}