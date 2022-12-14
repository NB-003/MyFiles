#pragma once

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "Map.h"

enum SelectedLevel { STEREO_MADNESS, BACK_ON_TRACK };
enum MenuMode { MAIN_MENU, LEVEL_SELECTION_MENU, CUBESPRITE_SELECTION_MENU };

class Menu {
public:
	Menu();
	void runMenuProcess(sf::View&, sf::RenderWindow&);

	SelectedLevel getSelectedLevel();

private:
	Map map;
	sf::Music music;

	sf::Texture launchSheetT, playButtonT, cubeChooseButtonT, level1ChooseT, level2ChooseT;
	sf::Sprite launchSheet, playButton, cubeChooseButton, level1Choose, level2Choose;

	bool isMenu = false;
	bool isSelected = false;

	MenuMode menuMode;
	enum MainMenuButton { NOT_A_BUTTON, PLAY_BUTTON, CUBE_CHOOSE_BUTTON } mainMenuButton;
	SelectedLevel selectedLevel;

	void RunMainMenuProcess(sf::View&, sf::RenderWindow&);
	void RunLevelSelectionMenuProcess(sf::View&, sf::RenderWindow&);
	void SetMusic();
};