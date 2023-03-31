package calculator.commands;

import java.util.Map;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Print implements Command {
    private final Stack<Double> stack;

    private static final Logger LOGGER = LogManager.getLogger(Print.class);

    public Print(String[] args, Stack<Double> stack, Map<String, Double> defines) {
        this.stack = stack;

        if (args.length != 1) {
            LOGGER.error("Invalid number of arguments for command 'PRINT': {}", args.length);
            throw new IllegalArgumentException("Command 'PRINT' requires no arguments");
        }
    }

    @Override
    public void execute() throws IllegalAccessException {
        if (stack.isEmpty()) {
            LOGGER.error("Stack is empty, nothing to print");
            throw new IllegalAccessException("Stack is empty, nothing to print");
        }

        double value = stack.peek();
        System.out.println(stack.peek());

        LOGGER.info("Executed command 'PRINT': {}", value);
    }
}
