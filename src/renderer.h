#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H

#include <SFML/Graphics.hpp>
#include <string>

class Renderer {
private:
	const unsigned int TILE_PADDING = 3;

	sf::RenderWindow* window;
	float tileSize = 50;
	sf::Vector2i dimensionsInTiles;
	sf::Vector2f tileStartPos;
	sf::Vector2f cameraShift;
	sf::Vector2f cameraPos;

	void onResize();
	void onCameraMove();

	void draw();

public:
	Renderer(sf::RenderWindow& w);
	Renderer(unsigned int w, unsigned int h, const std::string& title, const int style);
	~Renderer();
	void onStartup();
	void setCameraPos(sf::Vector2f pos);
	void tick();
	bool windowIsOpen();
};

#endif // CHESS_RENDERER_H
