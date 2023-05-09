package factory.utils;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

public class Statistics {
    private final Map<Class<?>, Integer> suppliedParts;
    private final AtomicInteger totalCarsAssembled;
    private final AtomicInteger totalCarsSold;

    public Statistics() {
        suppliedParts = new ConcurrentHashMap<>();
        totalCarsAssembled = new AtomicInteger(0);
        totalCarsSold = new AtomicInteger(0);
    }

    public void incrementSuppliedParts(Class<?> partType) {
        suppliedParts.putIfAbsent(partType, 0);
        suppliedParts.computeIfPresent(partType, (type, count) -> count + 1);
    }
    public void incrementCarsAssembled() {
        totalCarsAssembled.incrementAndGet();
    }

    public void incrementCarsSold() {
        totalCarsSold.incrementAndGet();
    }

    public String getStatistics() {
        StringBuilder statistics = new StringBuilder();
        statistics.append("Supplied parts:\n");
        for (Map.Entry<Class<?>, Integer> entry : suppliedParts.entrySet()) {
            statistics.append("      ")
                    .append(entry.getKey().getSimpleName())
                    .append(": ")
                    .append(entry.getValue())
                    .append("\n");
        }
        statistics.append("Cars assembled: ").append(totalCarsAssembled.get()).append("\n");
        statistics.append("Cars sold: ").append(totalCarsSold.get()).append("\n");

        return statistics.toString();
    }
}
