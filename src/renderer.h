#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H

#include <SFML/Graphics.hpp>
#include <string>

class Renderer {
private:
	const unsigned int TILE_PADDING = 3;
	const float MIN_TILE_SIZE = 5;
	const float MAX_TILE_SIZE = 300;
	unsigned int parity;

	sf::RenderWindow* window;
	float tileSize = 50;
	sf::Vector2u dimensionsInTiles;
	sf::Vector2f tileStartPos;
	sf::Vector2f cameraShift;
	sf::Vector2f cameraPos;

public:
	Renderer(sf::RenderWindow* w);
	~Renderer();
	void onStartup();
	void onResize();
	void onCameraMove();
	void onZoom(float delta);
	void setCameraPos(sf::Vector2f pos);
	void moveCamera(sf::Vector2f translationVec);
	void draw();
};

#endif // CHESS_RENDERER_H
