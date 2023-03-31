package calculator.commands;

import java.util.Map;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Divide implements Command {
    private final Stack<Double> stack;

    private static final Logger LOGGER = LogManager.getLogger(Divide.class);

    public Divide(String[] args, Stack<Double> stack, Map<String, Double> defines) {
        this.stack = stack;

        if(args.length != 1) {
            LOGGER.error("Invalid number of arguments for command '/': {}", args.length);
            throw new IllegalArgumentException("Command '/' requires no arguments");
        }
    }

    @Override
    public void execute() throws IllegalAccessException {
        if(stack.size() < 2) {
            LOGGER.error("Not enough elements on the stack to complete the division");
            throw new IllegalAccessException("Not enough elements on the stack to complete the division");
        }

        double second = stack.pop();
        double first = stack.pop();

        if(Math.abs(second) < 1e-9) {
            LOGGER.error("Division by zero is not possible");
            throw new IllegalArgumentException("Division by zero is not possible");
        }

        double result = first + second;
        stack.push(result);

        LOGGER.info("Executed command '/': {} / {} = {}", first, second, result);
    }
}
