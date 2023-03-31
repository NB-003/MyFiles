package calculator.commands;

import java.util.Map;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Multiply implements Command {
    private final Stack<Double> stack;

    private static final Logger LOGGER = LogManager.getLogger(Multiply.class);

    public Multiply(String[] args, Stack<Double> stack, Map<String, Double> defines) {
        this.stack = stack;

        if(args.length != 1) {
            LOGGER.error("Invalid number of arguments for command '*': {}", args.length);
            throw new IllegalArgumentException("Command '*' requires no arguments");
        }
    }

    @Override
    public void execute() throws IllegalAccessException {
        if(stack.size() < 2) {
            LOGGER.error("Not enough elements on the stack to complete the multiplication");
            throw new IllegalAccessException("Not enough elements on the stack to complete the multiplication");
        }

        double second = stack.pop();
        double first = stack.pop();

        double result = first + second;
        stack.push(result);

        LOGGER.info("Executed command '*': {} * {} = {}", first, second, result);
    }
}
