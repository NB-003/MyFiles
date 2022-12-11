#include "Level.h"

using namespace sf;
using namespace std;

int main() {
	View view;

	int windowWidth = 854, windowHeight = 480, mapHeight = 18 * 32, levelLength = 852 * 32;
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Geometry Dash");

	view.setSize(windowWidth, windowHeight);
	view.setCenter(windowWidth / 2.f, (2 * mapHeight - 288) / 2.f);

	TmxLevel levelMap;

	Level StereoMadness("Resources/tiles/map4.tmx", "Resources/StereoMadness.ogg", levelLength, mapHeight);
	StereoMadness.runLevelProcess(view, window);
	return 0;
}