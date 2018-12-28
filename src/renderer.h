#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H

#include <SFML/Graphics.hpp>
#include <string>

class Renderer {
private:
	// Configuration constants
	const unsigned int TILE_PADDING  = 3;
	const unsigned int MAX_FRAMERATE = 60;
	const float MIN_TILE_SIZE =   5.f;
	const float MAX_TILE_SIZE = 300.f;
	const sf::Color BACKGROUND_COLOR = sf::Color( 0,  0,  0, 255);
	const sf::Color FOREGROUND_COLOR = sf::Color(50, 50, 50, 255);

	// Resources
	const std::string FONT_DIRECTORY      = "res/font/";
	const std::string DEBUG_FONT_FILENAME = "OpenSans-Regular.ttf";
	sf::Font debugFont;

	// Flags
	bool displayDebugData = true;

	// Members
	sf::RenderWindow* window;

	unsigned int parity;
    float tileSize = 50;

	sf::Vector2u dimensionsInTiles;
	sf::Vector2f tileStartPos;
	sf::Vector2f cameraShift;
	sf::Vector2f cameraPos;

	// Utility methods
	void drawDebugText(const std::string& s, const unsigned int row);
	void draw(const int x, const int y, const sf::Color c);
	void drawDebug();

public:
	// Flags
    bool needsRedraw = false;

    // Constructors
    Renderer(sf::RenderWindow* w);
	~Renderer();

	// Event handlers
	void onStartup();
	void onResize();
	void onCameraMove();
	void onZoom(const float delta);

	// Accessors
	sf::Vector2f getMousePosition() const;

	// Mutators
	void setCameraPos(const sf::Vector2f pos);
	void moveCamera(const sf::Vector2f translationVec);
    void toggleDisplayDebugData();

    // Utility methods
	void draw();
};

#endif // CHESS_RENDERER_H
