package calculator.commands;

import java.util.Map;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Pop implements Command {
    private final Stack<Double> stack;

    private static final Logger LOGGER = LogManager.getLogger(Pop.class);

    public Pop(String[] args, Stack<Double> stack, Map<String, Double> defines) {
        this.stack = stack;

        if (args.length != 1) {
            LOGGER.error("Invalid number of arguments for command 'POP': {}", args.length);
            throw new IllegalArgumentException("Command 'POP' requires no arguments");
        }
    }

    @Override
    public void execute() throws IllegalAccessException {
        if (stack.isEmpty()) {
            LOGGER.error("Stack is empty, nothing to pop");
            throw new IllegalAccessException("Stack is empty, nothing to pop");
        }

        double value = stack.pop();
        LOGGER.info("Executed command 'POP': {}", value);
    }
}
