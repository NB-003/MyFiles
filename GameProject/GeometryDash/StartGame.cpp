#include "Level.h"
#include "Menu.h"

using namespace sf;
using namespace std;

void StartGame() {
	View view;

	int windowWidth = 854, windowHeight = 480, mapHeight1 = 13 * 32, mapHeight2 = 18 * 32;
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Geometry Dash");

	view.setSize(windowWidth, windowHeight);
	view.setCenter(windowWidth / 2.f, windowHeight / 2.f);

	Menu menu;

	while (window.isOpen()) {
		view.setCenter(windowWidth / 2.f, windowHeight / 2.f);
		menu.runMenuProcess(view, window);

		if (menu.getSelectedLevel() == STEREO_MADNESS) {
			view.setCenter(windowWidth / 2.f, (2 * mapHeight1 - 288) / 2.f);
			Level StereoMadness("Resources/tiles/StereoMadness.tmx", "Resources/StereoMadness.ogg", 465 * 32, 13 * 32);
			StereoMadness.runLevelProcess(view, window, sf::Color::Blue, sf::Color::Blue);
		}
		else if (menu.getSelectedLevel() == BACK_ON_TRACK) {
			view.setCenter(windowWidth / 2.f, (2 * mapHeight2 - 288) / 2.f);
			Level BackOnTrack("Resources/tiles/map4.tmx", "Resources/BackOnTrack.ogg", 200 * 32, mapHeight2);
			BackOnTrack.runLevelProcess(view, window, sf::Color::Red, sf::Color(255, 0, 100));
		}
	}
}