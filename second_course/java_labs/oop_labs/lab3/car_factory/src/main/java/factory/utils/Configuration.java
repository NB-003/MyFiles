package factory.utils;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class Configuration {
    private final Properties properties;

    public Configuration(String configFilePath) throws IOException {
        properties = new Properties();
        try (FileInputStream input = new FileInputStream(configFilePath)) {
            properties.load(input);
        }
    }

    public int getIntProperty(String key) {
        return Integer.parseInt(properties.getProperty(key));
    }

    public long getLongProperty(String key) {
        return Long.parseLong(properties.getProperty(key));
    }

    public boolean getPropertyAsBoolean(String key) {
        return Boolean.parseBoolean(properties.getProperty(key));
    }
}
