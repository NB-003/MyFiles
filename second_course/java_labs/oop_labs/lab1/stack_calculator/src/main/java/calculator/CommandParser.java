package calculator;

import calculator.commands.Command;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class CommandParser {
    private final CommandFactory commandFactory;

    private static final Logger LOGGER = LogManager.getLogger(CommandParser.class);
    private static final Logger COMMAND_LOGGER = LogManager.getLogger(Command.class);

    public CommandParser() {
        commandFactory = new CommandFactory();
    }

    public Command parse(String line) {
        String[] args = line.split("\\s+");
        if (args.length == 0 || args[0].startsWith("#")) {
            return null;
        }

        String commandName = args[0].toUpperCase();

        try {
            Command command = commandFactory.createCommand(commandName, args);
            if (command != null) {
                COMMAND_LOGGER.info("Executed command: {}", commandName);
            }
            return command;
        } catch (Exception e) {
            LOGGER.error("Error creating command: {}", commandName, e);
            return null;
        }
    }
}
