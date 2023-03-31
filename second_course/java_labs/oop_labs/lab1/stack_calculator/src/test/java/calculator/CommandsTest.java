package calculator;

import calculator.commands.*;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Map;
import java.util.HashMap;
import java.util.Stack;

import static org.junit.jupiter.api.Assertions.*;

public class CommandsTest {
    private Stack<Double> stack;
    private Map<String, Double> defines;
    private CommandParser commandParser;
    private Command command;

    @BeforeEach
    void setUp() {
        stack = new Stack<>();
        defines = new HashMap<>();
        commandParser = new CommandParser();
    }

    @Test
    public void testAddCommand() throws IllegalAccessException {
        command = commandParser.parse("+");

        stack.push(2.0);
        stack.push(3.0);
        command.execute();

        assertEquals(1, stack.size());
        assertEquals(5.0, stack.peek());
    }

    @Test
    public void testSubtractCommand() throws IllegalAccessException {
        command = commandParser.parse("-");

        stack.push(5.0);
        stack.push(3.0);
        command.execute();

        assertEquals(1, stack.size());
        assertEquals(2.0, stack.peek());
    }

    @Test
    public void testMultiplyCommand() {

    }

    @Test
    public void testDivideCommand() {

    }

    @Test
    public void testSqrtCommand() {

    }

    @Test
    public void testPrintCommand() {

    }

    @Test
    public void testDefineCommand() {

    }

    @Test
    public void testPushCommand() {

    }

    @Test
    public void testPopCommand() {

    }
}

