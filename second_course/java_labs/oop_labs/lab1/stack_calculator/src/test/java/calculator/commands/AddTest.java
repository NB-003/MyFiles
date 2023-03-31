package calculator.commands;

import calculator.CommandParser;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Map;
import java.util.HashMap;
import java.util.Stack;

public class AddTest {
    @BeforeEach
    void setUp() {
        stack = new Stack<>();
        defines = new HashMap<>();
        commandParser = new CommandParser();
    }
}
