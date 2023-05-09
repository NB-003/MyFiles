package factory.factory.suppliers;

import factory.factory.details.Body;
import factory.factory.DetailStorage;
import factory.utils.Statistics;

public class BodySupplier extends DetailSupplier<Body> {
    public BodySupplier(Statistics statistics, DetailStorage<Body> storage, long productionTime) {
        super(statistics, storage, productionTime);
    }

    @Override
    protected Body createCarPart(long id) {
        return new Body(id);
    }
}
