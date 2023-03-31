package calculator;

import calculator.commands.Command;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class Calculator {
    private final CommandParser commandParser;

    private static final Logger LOGGER = LogManager.getLogger(Calculator.class);

    public static void main(String[] args) {
        Calculator calculator = new Calculator();

        if (args.length == 0) {
            calculator.executeFromCommandLine();
        } else if (args.length == 1) {
            calculator.executeFromFile(args[0]);
        } else {
            throw new IllegalArgumentException("Too many arguments, expected no more than 2");
        }
    }

    public Calculator() {
        commandParser = new CommandParser();
    }

    private void executeCommand(String line) {
        try {
            Command command = commandParser.parse(line);
            if (command != null) {
                command.execute();
                LOGGER.info("Executed command: {}", line);
            }
        } catch (IllegalAccessException e) {
            LOGGER.error("Error executing command: {}", line, e);
        }
    }

    public void executeFromCommandLine() {
        LOGGER.info("Starting calculator from command line input");
        System.out.println("Enter the commands for the calculator (to exit the program, type 'QUIT'):");

        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            if (line.equalsIgnoreCase("QUIT")) {
                break;
            }
            executeCommand(line);
        }

        LOGGER.info("Exiting calculator from command line input");
    }

    public void executeFromFile(String fileName) {
        LOGGER.info("Executing commands from file: {}", fileName);

        File file = new File(fileName);
        try (Scanner scanner = new Scanner(file)) {
            while (scanner.hasNextLine()) {
                String line = scanner.nextLine();
                executeCommand(line);
            }
        } catch (FileNotFoundException e) {
            LOGGER.error("File not found: {}", fileName, e);
        }

        LOGGER.info("Finished executing commands from file: {}", fileName);
    }
}
