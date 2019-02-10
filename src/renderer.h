#ifndef CHESS_RENDERER_H
#define CHESS_RENDERER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "game.h"

// Forward declarations
class Game;
class InputHandler;
class MoveMarker;
class Piece;
class WindowLayer;


// Class declaration
class Renderer {
private:
	// Configuration constants
	const unsigned int TILE_PADDING  = 3;
	const unsigned int MAX_FRAMERATE = 60;
	const float MIN_TILE_SIZE =  16.f;
	const float MAX_TILE_SIZE = 256.f;

	// Resources
	const std::string FONT_DIRECTORY      = "res/font/";
	const std::string DEBUG_FONT_FILENAME = "OpenSans-Regular.ttf";
	sf::Font debugFont;

	const std::string WINDOW_ICON = "King";

	const sf::Color BACKGROUND_COLOR     = sf::Color(  0,   0,   0, 255);
	const sf::Color FOREGROUND_COLOR     = sf::Color( 50,  50,  50, 255);
	const sf::Color MOUSE_DEBUG_COLOR    = sf::Color(255, 255, 255, 100);
	const sf::Color MOUSE_VALID_COLOR    = sf::Color(  0, 255,   0, 100);
	const sf::Color MOUSE_INVALID_COLOR  = sf::Color(255,   0,   0, 100);
	const sf::Color PIECE_SELECTED_COLOR = sf::Color(255, 255,   0, 100);
	const sf::Color MOVE_MARKER_COLOR    = sf::Color(  0, 255, 255, 100);

	// Flags
	bool displayDebugData = false;
	bool displayMenu = false;

	// Members
	Game* game;
	sf::RenderWindow* window;

	std::map<std::string, sf::Texture*>* textures;
	std::map<std::string, sf::Texture*>* uiTextures;
	std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams;
	std::vector<WindowLayer*> layers;

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
	void drawUILayers() const;

	void drawDebugText(const std::string& s, const unsigned int row) const;
	void drawPiece(Piece* p) const;
	void drawTile(const int x, const int y, const sf::Color c) const;
	void drawDebug() const;

public:
	// Flags
	bool needsRedraw = false;

	// Constructors
	Renderer(Game* g, sf::RenderWindow* w);
	~Renderer();

	// Event handlers
	void onStartup(
		std::map<std::string, sf::Texture*>* textures_,
		std::map<std::string, sf::Texture*>* uiTextures_,
		std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams_
	);
	void onResize(const unsigned int width, const unsigned int height);
	void onCameraMove();
	void onZoom(const float delta);

	// Accessors
	sf::Vector2f getMousePosition() const;
	sf::Vector2i getMouseTilePosition() const;
	sf::Vector2u getTileDimensions() const;
	bool shouldGenerate(MoveMarker* terminal) const;
	inline bool menuIsVisible() const {
        return displayMenu;
	};

	// Mutators
	void setCameraPos(const sf::Vector2f pos);
	void moveCamera(const sf::Vector2f translationVec);
	void toggleDisplayDebugData();
	void toggleMenu();

	void addLayer(WindowLayer* layer) {
		layers.push_back(layer);
	}

	// Utility methods
	void draw();
};

#endif // CHESS_RENDERER_H
