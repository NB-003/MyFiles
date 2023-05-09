module factory {
    requires javafx.controls;
    requires javafx.fxml;
    requires org.apache.logging.log4j;
    requires org.apache.logging.log4j.core;


    opens factory to javafx.fxml;
    exports factory;
    exports factory.factory;
    opens factory.factory to javafx.fxml;
    exports factory.factory.suppliers;
    opens factory.factory.suppliers to javafx.fxml;
    exports factory.factory.details;
    opens factory.factory.details to javafx.fxml;
    exports factory.utils;
    opens factory.utils to javafx.fxml;
}