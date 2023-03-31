package calculator.commands;

import java.util.Map;
import java.util.Stack;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class SquareRoot implements Command {
    private final Stack<Double> stack;

    private static final Logger LOGGER = LogManager.getLogger(Multiply.class);

    public SquareRoot(String[] args, Stack<Double> stack, Map<String, Double> defines) {
        this.stack = stack;

        if(args.length != 1) {
            LOGGER.error("Invalid number of arguments for command 'SQRT': {}", args.length);
            throw new IllegalArgumentException("Command 'SQRT' requires no arguments");
        }
    }

    @Override
    public void execute() throws IllegalAccessException {
        if (stack.isEmpty()) {
            LOGGER.error("Not enough elements on the stack to complete the square root operation");
            throw new IllegalAccessException("Not enough elements on the stack to complete the square root operation");
        }

        double operand = stack.pop();

        if (operand < 0) {
            LOGGER.error("Square root of a negative number ({}) is undefined", operand);
            throw new IllegalArgumentException("Square root of a negative number is undefined");
        }

        double result = Math.sqrt(operand);
        stack.push(result);

        LOGGER.info("Executed command 'SQRT': √{} = {}", operand, result);
    }
}
