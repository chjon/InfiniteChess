#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "game.h"

// Forward declarations
class Game;
class InputHandler;
class GamePiece;



// Class declaration
class Renderer {
private:
	// Configuration constants
	const unsigned int TILE_PADDING  = 3;
	const unsigned int MAX_FRAMERATE = 60;
	const float MIN_TILE_SIZE =  20.f;
	const float MAX_TILE_SIZE = 120.f;

	// Resources
	const std::string FONT_DIRECTORY      = "res/font/";
	const std::string DEBUG_FONT_FILENAME = "OpenSans-Regular.ttf";
	sf::Font debugFont;

	const sf::Color BACKGROUND_COLOR     = sf::Color(  0,   0,   0, 255);
	const sf::Color FOREGROUND_COLOR     = sf::Color( 50,  50,  50, 255);
	const sf::Color MOUSE_DEBUG_COLOR    = sf::Color(255, 255, 255, 100);
	const sf::Color MOUSE_VALID_COLOR    = sf::Color(  0, 255,   0, 100);
	const sf::Color MOUSE_INVALID_COLOR  = sf::Color(255,   0,   0, 100);
	const sf::Color PIECE_SELECTED_COLOR = sf::Color(255, 255,   0, 100);
	const sf::Color MOVE_MARKER_COLOR    = sf::Color(  0, 255, 255, 100);

	// Flags
	bool displayDebugData = true;

	// Members
	Game* game;
	sf::RenderWindow* window;

	unsigned int parity;
	float tileSize = 50;

	sf::Vector2u dimensionsInTiles;
	sf::Vector2f tileStartPos;
	sf::Vector2f cameraShift;
	sf::Vector2f cameraPos;

	// Utility methods
	sf::Vector2i getScreenPos(sf::Vector2i pos) const;

	void drawBoard() const;
	void drawPieces() const;
	void drawOverlays() const;

	void drawDebugText(const std::string& s, const unsigned int row) const;
	void drawPiece(GamePiece* p) const;
	void drawTile(const int x, const int y, const sf::Color c) const;
	void drawDebug() const;
public:
	// Flags
	bool needsRedraw = false;

	// Constructors
	Renderer(Game* g, sf::RenderWindow* w);
	~Renderer();

	// Event handlers
	void onStartup();
	void onResize(const unsigned int width, const unsigned int height);
	void onCameraMove();
	void onZoom(const float delta);

	// Accessors
	sf::Vector2f getMousePosition() const;
	sf::Vector2i getMouseTilePosition() const;
	sf::Vector2u getTileDimensions() const;
	bool isRenderable(sf::Vector2i pos) const;

	// Mutators
	void setCameraPos(const sf::Vector2f pos);
	void moveCamera(const sf::Vector2f translationVec);
	void toggleDisplayDebugData();

	// Utility methods
	void draw();
};

#endif // CHESS_RENDERER_H
