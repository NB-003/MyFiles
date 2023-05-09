package factory;

import factory.factory.details.Accessory;
import factory.factory.details.Body;
import factory.factory.details.Engine;
import factory.factory.suppliers.AccessorySupplier;
import factory.factory.suppliers.BodySupplier;
import factory.factory.suppliers.DetailSupplier;
import factory.factory.suppliers.EngineSupplier;
import factory.utils.Configuration;
import factory.utils.Logger;
import factory.utils.Statistics;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;
import factory.factory.*;
import factory.threadpool.ThreadPool;
import javafx.util.Duration;

import java.util.ArrayList;
import java.util.List;

public class FactoryApp extends Application {
    private ThreadPool threadPool;

    private List<DetailSupplier<?>> suppliers;
    private CarAssembler carAssembler;
    private List<CarDealer> dealers;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        Configuration config = new Configuration("src/main/resources/config.properties");

        int bodyStorageCapacity = config.getIntProperty("body.storage.capacity");
        int engineStorageCapacity = config.getIntProperty("engine.storage.capacity");
        int accessoryStorageCapacity = config.getIntProperty("accessory.storage.capacity");
        int carStorageCapacity = config.getIntProperty("car.storage.capacity");
        int threadPoolSize = config.getIntProperty("thread.pool.size");

        DetailStorage<Body> bodyStorage = new DetailStorage<>(bodyStorageCapacity);
        DetailStorage<Engine> engineStorage = new DetailStorage<>(engineStorageCapacity);
        DetailStorage<Accessory> accessoryStorage = new DetailStorage<>(accessoryStorageCapacity);
        CarStorage carStorage = new CarStorage(carStorageCapacity);

        threadPool = new ThreadPool(threadPoolSize);

        // Создание и запуск поставщиков, сборщиков и дилеров
        Statistics statistics = new Statistics();

        suppliers = new ArrayList<>();
        suppliers.add(new BodySupplier(statistics, bodyStorage, config.getLongProperty("body.supplier.production.time")));
        suppliers.add(new EngineSupplier(statistics, engineStorage, config.getLongProperty("engine.supplier.production.time")));
        suppliers.add(new AccessorySupplier(statistics, accessoryStorage, config.getLongProperty("accessory.supplier.production.time")));
        suppliers.forEach(threadPool::execute);

        carAssembler = new CarAssembler(statistics, bodyStorage, engineStorage, accessoryStorage, carStorage, config.getLongProperty("car.assembler.time"));
        threadPool.execute(carAssembler);

        dealers = new ArrayList<>();
        for (int i = 0; i < config.getIntProperty("car.dealer.number"); i++) {
            CarDealer dealer = new CarDealer(statistics, i, carStorage, config.getLongProperty("car.dealer.request.time"));
            dealers.add(dealer);
            threadPool.execute(dealer);
        }

        GridPane grid = new GridPane();
        grid.setAlignment(Pos.CENTER);
        grid.setHgap(10);
        grid.setVgap(10);
        grid.setPadding(new Insets(25, 25, 25, 25));

        // Создание элементов интерфейса и их добавление в grid
        Label titleLabel = new Label("Car Factory");
        titleLabel.setStyle("-fx-font-size: 24;");
        grid.add(titleLabel, 0, 0, 2, 1);

        Slider bodySlider = new Slider(500, 2000, config.getLongProperty("body.supplier.production.time"));
        bodySlider.setShowTickLabels(true);
        bodySlider.setShowTickMarks(true);
        bodySlider.setMajorTickUnit(500);
        bodySlider.setBlockIncrement(100);
        HBox bodySliderBox = new HBox(10);
        bodySliderBox.setAlignment(Pos.CENTER);
        bodySliderBox.getChildren().addAll(new Label("Body Supplier Time (ms):"), bodySlider);
        grid.add(bodySliderBox, 0, 1);

        Slider engineSlider = new Slider(500, 2000, config.getLongProperty("engine.supplier.production.time"));
        engineSlider.setShowTickLabels(true);
        engineSlider.setShowTickMarks(true);
        engineSlider.setMajorTickUnit(500);
        engineSlider.setBlockIncrement(100);
        HBox engineSliderBox = new HBox(10);
        engineSliderBox.setAlignment(Pos.CENTER);
        engineSliderBox.getChildren().addAll(new Label("Engine Supplier Time (ms):"), engineSlider);
        grid.add(engineSliderBox, 0, 2);

        Slider accessorySlider = new Slider(500, 2000, config.getLongProperty("accessory.supplier.production.time"));
        accessorySlider.setShowTickLabels(true);
        accessorySlider.setShowTickMarks(true);
        accessorySlider.setMajorTickUnit(500);
        accessorySlider.setBlockIncrement(100);
        HBox accessorySliderBox = new HBox(10);
        accessorySliderBox.setAlignment(Pos.CENTER);
        accessorySliderBox.getChildren().addAll(new Label("Accessory Supplier Time (ms):"), accessorySlider);
        grid.add(accessorySliderBox, 0, 3);

        Slider assemblerSlider = new Slider(1000, 4000, config.getLongProperty("car.assembler.time"));
        assemblerSlider.setShowTickLabels(true);
        assemblerSlider.setShowTickMarks(true);
        assemblerSlider.setMajorTickUnit(1000);
        assemblerSlider.setBlockIncrement(100);
        HBox assemblerSliderBox = new HBox(10);
        assemblerSliderBox.setAlignment(Pos.CENTER);
        assemblerSliderBox.getChildren().addAll(new Label("Car Assembler Time (ms):"), assemblerSlider);
        grid.add(assemblerSliderBox, 0, 4);

        Slider dealerSlider = new Slider(1000, 5000, config.getLongProperty("car.dealer.request.time"));
        dealerSlider.setShowTickLabels(true);
        dealerSlider.setShowTickMarks(true);
        dealerSlider.setMajorTickUnit(1000);
        dealerSlider.setBlockIncrement(100);
        HBox dealerSliderBox = new HBox(10);
        dealerSliderBox.setAlignment(Pos.CENTER);
        dealerSliderBox.getChildren().addAll(new Label("Car Dealer Request Time (ms):"), dealerSlider);
        grid.add(dealerSliderBox, 0, 5);

        // Создание текстовых полей для отображения статистики и логов
        TextArea statisticsArea = new TextArea();
        statisticsArea.setEditable(false);
        statisticsArea.setPrefSize(400, 200);
        statisticsArea.setWrapText(true);

        TextArea logTextArea = new TextArea();
        logTextArea.setEditable(false);
        logTextArea.setPrefSize(400, 200);
        logTextArea.setWrapText(true);

        Timeline statisticsTimeline = new Timeline(new KeyFrame(Duration.millis(config.getLongProperty("statistics.update.time")), event -> updateStatistics(statisticsArea, statistics)));
        statisticsTimeline.setCycleCount(Timeline.INDEFINITE);
        statisticsTimeline.play();

        Logger.configureLogger(config);
        Timeline logTimeline = new Timeline(new KeyFrame(Duration.millis(config.getLongProperty("logging.update.time")), event -> updateLogs(logTextArea)));
        logTimeline.setCycleCount(Timeline.INDEFINITE);
        logTimeline.play();

        VBox textAreasBox = new VBox(10);
        textAreasBox.getChildren().addAll(statisticsArea, logTextArea);
        grid.add(textAreasBox, 1, 1, 1, 6);

        Button applyChangesButton = new Button("Apply Changes");
        applyChangesButton.setOnAction(e -> {
            suppliers.get(0).updateProductionTime((long) bodySlider.getValue());
            suppliers.get(1).updateProductionTime((long) engineSlider.getValue());
            suppliers.get(2).updateProductionTime((long) accessorySlider.getValue());
            carAssembler.updateAssemblyTime((long) assemblerSlider.getValue());
            dealers.forEach(dealer -> dealer.updateRequestTime((long) dealerSlider.getValue()));
        });
        grid.add(applyChangesButton, 0, 6);

        primaryStage.setOnCloseRequest(e -> {
            stop();
            Platform.exit();
        });

        primaryStage.setTitle("Car Factory");
        primaryStage.setScene(new Scene(grid, 800, 600));
        primaryStage.show();
    }

    @Override
    public void stop() {
        // Остановка всех работающих потоков и завершение работы пула потоков
        suppliers.forEach(DetailSupplier::stop);
        carAssembler.stop();
        dealers.forEach(CarDealer::stop);
        threadPool.shutdown();
    }

    private void updateStatistics(TextArea statisticsArea, Statistics statistics) {
        statisticsArea.setText(statistics.getStatistics());
    }

    private void updateLogs(TextArea logTextArea) {
        String logContent = Logger.readLogFile();
        logTextArea.setText(logContent);
    }
}
