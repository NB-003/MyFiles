package chat.serialization;

import com.google.gson.Gson;

import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class StreamHandler {
    private final Scanner scanner;
    private final PrintWriter writer;
    private final Socket socket;
    private final Gson jsonSerializer;

    public StreamHandler(Socket socket, Gson gson) throws IOException {
        scanner = new Scanner(socket.getInputStream());
        writer = new PrintWriter(socket.getOutputStream(), true);
        this.socket = socket;
        jsonSerializer = gson;
    }

    public Scanner getScanner() {
        return scanner;
    }

    public PrintWriter getWriter() {
        return writer;
    }

    public Socket getSocket() {
        return socket;
    }

    public Gson getJsonSerializer() {
        return jsonSerializer;
    }
}
