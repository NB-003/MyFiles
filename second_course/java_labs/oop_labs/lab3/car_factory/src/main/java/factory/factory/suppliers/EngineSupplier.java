package factory.factory.suppliers;

import factory.factory.DetailStorage;
import factory.factory.details.Engine;
import factory.utils.Statistics;

public class EngineSupplier extends DetailSupplier<Engine> {
    public EngineSupplier(Statistics statistics, DetailStorage<Engine> storage, long productionTime) {
        super(statistics, storage, productionTime);
    }

    @Override
    protected Engine createCarPart(long id) {
        return new Engine(id);
    }
}
