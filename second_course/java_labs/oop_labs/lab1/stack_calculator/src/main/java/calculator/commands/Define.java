package calculator.commands;

import java.util.Map;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Define implements Command {
    private final Map<String, Double> defines;
    private String name;
    private final double value;

    private static final Logger LOGGER = LogManager.getLogger(Define.class);

    public Define(String[] args, Stack<Double> stack, Map<String, Double> defines) {
        this.defines = defines;
        name = args[1];

        if (args.length != 3) {
            LOGGER.error("Invalid number of arguments for command 'DEFINE': {}", args.length);
            throw new IllegalArgumentException("Command 'DEFINE' requires a variable name and a value argument");
        }

        if (Character.isDigit(name.charAt(0))) {
            LOGGER.error("Variable name should not be started with a digit: {}", name);
            throw new IllegalArgumentException("Variable name should not be started with a digit");
        } else if (defines.containsKey(name)) {
            LOGGER.error("Variable named '{}' already exists", name);
            throw new IllegalArgumentException("Variable named '" + name + "' already exists");
        } else {
            name = args[1];
        }

        try {
            value = Double.parseDouble(args[2]);
        } catch (NumberFormatException e) {
            LOGGER.error("Invalid number format for value argument: {}", args[2]);
            throw new IllegalArgumentException("Invalid number format for value argument");
        }
    }

    @Override
    public void execute() {
        defines.put(name, value);
        LOGGER.info("Executed command 'DEFINE': {} = {}", name, value);
    }
}
