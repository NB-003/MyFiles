package factory.factory.suppliers;

import factory.factory.details.Accessory;
import factory.factory.DetailStorage;
import factory.utils.Statistics;

public class AccessorySupplier extends DetailSupplier<Accessory> {
    public AccessorySupplier(Statistics statistics, DetailStorage<Accessory> storage, long productionTime) {
        super(statistics, storage, productionTime);
    }

    @Override
    protected Accessory createCarPart(long id) {
        return new Accessory(id);
    }
}
