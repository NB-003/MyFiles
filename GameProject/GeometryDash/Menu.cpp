#include "Menu.h"

Menu::Menu() {
	menuMode = MAIN_MENU; 
	mainMenuButton = NOT_A_BUTTON;
	selectedLevel = STEREO_MADNESS;

	launchSheetT.loadFromFile("Resources/LaunchSheet.png");
	playButtonT.loadFromFile("Resources/PlayButton.png");
	//cubeChooseButtonT.loadFromFile("Resources/CubeChooseButton.png");

	launchSheet.setTexture(launchSheetT);
	playButton.setTexture(playButtonT);
	//cubeChooseButton.setTexture(cubeChooseButtonT);

	level1ChooseT.loadFromFile("Resources/Level1Choose.png");
	level2ChooseT.loadFromFile("Resources/Level2Choose.png");

	level1Choose.setTexture(level1ChooseT);
	level2Choose.setTexture(level2ChooseT);
}

void Menu::runMenuProcess(sf::View& view, sf::RenderWindow& window) {
	isMenu = true;
	SetMusic();
	while (isMenu) {
		if (menuMode == LEVEL_SELECTION_MENU) {
			RunLevelSelectionMenuProcess(view, window);
		}
		if (menuMode == MAIN_MENU) {
			RunMainMenuProcess(view, window);
		}
	}
	music.stop();
}

void Menu::RunMainMenuProcess(sf::View& view, sf::RenderWindow& window) {
	launchSheet.setScale(0.75, 0.75);
	launchSheet.setOrigin(858 / 2.f, 110 / 2.f);
	launchSheet.setPosition(854 / 2.f, 80);

	playButton.setScale(0.8, 0.8);
	playButton.setOrigin(210 / 2.f, 208 / 2.f);
	playButton.setPosition(854 / 2.f, 480 / 2.f);

	map.getBackgroundSprite().setPosition(0, 0);
	map.getBackgroundSprite().setColor(sf::Color::Green);
	map.getGroundSprite().setColor(sf::Color::Yellow);

	while (menuMode == MAIN_MENU && window.isOpen()) {
		mainMenuButton = NOT_A_BUTTON;

		window.setView(view);
		window.clear(sf::Color::Blue);

		if (sf::IntRect((window.getSize().x - 158 / 854.f * window.getSize().x) / 2,
			(window.getSize().y - 156 / 480.f * window.getSize().y) / 2,
			158 / 854.f * window.getSize().x, 
			156 / 480.f * window.getSize().y).contains(sf::Mouse::getPosition(window)))
			mainMenuButton = PLAY_BUTTON;

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::MouseButtonReleased
				&& event.mouseButton.button == sf::Mouse::Left && mainMenuButton == PLAY_BUTTON) {
				menuMode = LEVEL_SELECTION_MENU;
				break;
			}

			if (event.type == sf::Event::Closed 
				||
				(event.type == sf::Event::KeyReleased 
				&& 
				event.key.code == sf::Keyboard::Escape)) {
					isMenu = false;
					music.stop();
					window.close();
			}
		}

		map.getBackgroundSprite().move(0.9, 0);
		window.draw(map.getBackgroundSprite());

		for (int i = 0; i < 8000; i++) {
			map.getGroundSprite().setPosition(i * 128, 384);
			window.draw(map.getGroundSprite());
		}

		view.move(1.1, 0);
		launchSheet.move(1.1, 0);
		playButton.move(1.1, 0);

		window.draw(launchSheet);
		window.draw(playButton);

		window.display();
	}
}

void Menu::RunLevelSelectionMenuProcess(sf::View& view, sf::RenderWindow& window) {
	view.reset(sf::FloatRect(0, 0, 854, 480));

	level1Choose.setScale(854 / 1920.f, 480 / 1080.f);
	level1Choose.setPosition(0, 0);

	level2Choose.setScale(854 / 1920.f, 480 / 1080.f);
	level2Choose.setPosition(0, 0);

	while (menuMode == LEVEL_SELECTION_MENU && window.isOpen()) {
		isSelected = false;

		window.setView(view);
		window.clear(sf::Color::Blue);
		if (sf::IntRect((window.getSize().x - 510 / 854.f * window.getSize().x) / 2, 
			77 / 480.f * window.getSize().y, 
			510 / 854.f * window.getSize().x, 
			140 / 480.f * window.getSize().y).contains(sf::Mouse::getPosition(window)))
			isSelected = true;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			if (isSelected) {
				isMenu = false;
				break;
			}
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
				menuMode = MAIN_MENU;
				break;
			}
		}

		if(selectedLevel == STEREO_MADNESS) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				selectedLevel = BACK_ON_TRACK;
				continue;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				selectedLevel = BACK_ON_TRACK;
				continue;
			}

			window.draw(level1Choose);
		}

		else if (selectedLevel == BACK_ON_TRACK) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				selectedLevel = STEREO_MADNESS;
				continue;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				selectedLevel = STEREO_MADNESS;
				continue;
			}

			window.draw(level2Choose);
		}
		window.display();
	}
}

SelectedLevel Menu::getSelectedLevel() { return selectedLevel; }

void Menu::SetMusic() {
	music.openFromFile("Resources/menuLoop.ogg");
	music.play();
	music.setLoop(true);
	music.setVolume(50.f);
}