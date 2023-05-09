package factory.factory;

import factory.factory.details.Accessory;
import factory.factory.details.Body;
import factory.factory.details.Engine;

public record Car(long id, Body body, Engine engine, Accessory accessory) { }
