package factory.utils;

import factory.factory.Car;
import org.apache.logging.log4j.Level;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.core.LoggerContext;
import org.apache.logging.log4j.core.appender.FileAppender;
import org.apache.logging.log4j.core.layout.PatternLayout;

import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.LocalDateTime;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public abstract class Logger {
    private static final String LOG_FILE_PATH = "src/main/resources/sales_log.log";
    private static boolean isLoggingEnabled = false;

    public static void configureLogger(Configuration config) {
        try {
            isLoggingEnabled = config.getPropertyAsBoolean("logging.enabled");
            if (!isLoggingEnabled) {
                LogManager.shutdown();
                return;
            }

            // Очистка лог-файла
            FileWriter fileWriter = new FileWriter(LOG_FILE_PATH, false);
            fileWriter.close();

            // Конфигурация логгера
            LoggerContext.getContext().getConfiguration().getRootLogger().setLevel(Level.INFO);
            PatternLayout layout = PatternLayout.newBuilder()
                    .withPattern("%d{yyyy-MM-dd HH:mm:ss} %-5p %c{1}:%L - %m%n")
                    .build();
            FileAppender fileAppender = FileAppender.newBuilder()
                    .setName("FileAppender")
                    .setLayout(layout)
                    .withFileName(LOG_FILE_PATH)
                    .withAppend(true) // Устанавливаем режим добавления новых логов в файл
                    .build();
            fileAppender.start();

            LoggerContext.getContext().getConfiguration().addAppender(fileAppender);
            LoggerContext.getContext().getConfiguration().getRootLogger().addAppender(fileAppender, null, null);
            LoggerContext.getContext().updateLoggers();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String readLogFile() {
        Path logPath = Paths.get(LOG_FILE_PATH);
        if (!Files.exists(logPath)) {
            try {
                Files.createFile(logPath);
            } catch (IOException e) {
                return "Error creating log file";
            }
        }

        try (Stream<String> lines = Files.lines(logPath)) {
            return lines.collect(Collectors.joining("\n"));
        } catch (IOException e) {
            return "Error reading log file";
        }
    }

    private static void prependLogToFile(String log) {
        try {
            RandomAccessFile raf = new RandomAccessFile(LOG_FILE_PATH, "rw");
            byte[] data = new byte[(int) raf.length()];
            raf.readFully(data);
            raf.seek(0);
            raf.writeBytes(log + System.lineSeparator());
            raf.write(data);
            raf.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void logCarSold(Car car, int dealerNumber) {
        if (!isLoggingEnabled) {
            return;
        }
        String log = String.format("%s: Dealer %d: Auto %d (Body: %d, Motor: %d, Accessory: %d)",
                LocalDateTime.now(), dealerNumber, car.id(), car.body().getId(),
                car.engine().getId(), car.accessory().getId());
        prependLogToFile(log);
    }
}
