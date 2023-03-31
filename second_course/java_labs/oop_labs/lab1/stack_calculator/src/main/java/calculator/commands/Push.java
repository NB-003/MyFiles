package calculator.commands;

import java.util.Map;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Push implements Command {
    private final Stack<Double> stack;
    private double value;

    private static final Logger LOGGER = LogManager.getLogger(Push.class);

    public Push(String[] args, Stack<Double> stack, Map<String, Double> defines) {
        this.stack = stack;

        if (args.length != 2) {
            LOGGER.error("Invalid number of arguments for command 'PUSH': {}", args.length);
            throw new IllegalArgumentException("Command 'PUSH' requires a value argument");
        }

        try {
            value = Double.parseDouble(args[1]);
        } catch (NumberFormatException e) {
            if (defines.containsKey(args[1])) {
                value = defines.get(args[1]);
            } else {
                LOGGER.error("Undefined variable: {}", args[1]);
                throw new IllegalArgumentException("Undefined variable: " + args[1]);
            }
        }
    }

    @Override
    public void execute() {
        stack.push(value);
        LOGGER.info("Executed command 'PUSH': {}", value);
    }
}
