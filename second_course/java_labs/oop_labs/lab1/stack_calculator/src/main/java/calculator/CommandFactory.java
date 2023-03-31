package calculator;

import calculator.commands.Command;

import java.io.InputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;


public class CommandFactory {
    private final Stack<Double> stack;
    private final Map<String, Double> defines;
    private final Map<String, String> commandMap;

    private static final Logger LOGGER = LogManager.getLogger(CommandFactory.class);

    public CommandFactory() {
        stack = new Stack<>();
        defines = new HashMap<>();
        commandMap = new HashMap<>();
        loadCommandsFromFile();
    }

    private void loadCommandsFromFile() {
        Properties properties = new Properties();
        final String propertiesFileName = "commands.properties";

        try (InputStream inputStream = getClass().getClassLoader().getResourceAsStream(propertiesFileName)) {
            if (inputStream == null) {
                LOGGER.error("File '{}' not found in resources folder", propertiesFileName);
                throw new FileNotFoundException("File '" + propertiesFileName + "' not found in resources folder");
            }
            properties.load(inputStream);
        } catch (IOException e) {
            LOGGER.error("Unable to read '{}' file", propertiesFileName, e);
            throw new RuntimeException("Unable to read '" + propertiesFileName + "' file", e);
        }

        for (String commandName : properties.stringPropertyNames()) {
            commandMap.put(commandName, properties.getProperty(commandName));
        }

        LOGGER.info("Loaded {} commands from '{}' file", commandMap.size(), propertiesFileName);
    }

    public Command createCommand(String commandName, String[] args) throws Exception {
        if (commandMap.containsKey(commandName)) {
            String commandClassName = commandMap.get(commandName);

            Class<?> commandClass = Class.forName(commandClassName);
            Command command = (Command) commandClass.getDeclaredConstructor(String[].class, Stack.class, Map.class).newInstance(args, stack, defines);

            LOGGER.info("Created command: {}", commandName);
            return command;
        } else {
            LOGGER.error("Invalid command: {}", commandName);
            throw new IllegalArgumentException("Invalid command: " + commandName);
        }
    }
}
