#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "inputHandler.h"
#include "pieceTracker.h"
#include "controller.h"
#include "resourceLoader.h"

// Forward declarations
class Renderer;
class InputHandler;
class PieceTracker;
class Controller;
class ResourceLoader;



// Class declaration
class Game {
private:
	// Members
    sf::RenderWindow* window;
	Renderer*         renderer;
	InputHandler*     inputHandler;
	PieceTracker*     pieceTracker;
	Controller*       controller;

	std::map<std::string, const PieceDef*>* pieceDefs;
	std::map<std::string, sf::Texture*>* textures;
	std::map<std::string, sf::Texture*>* uiTextures;

	// Friends
	friend Renderer;
	friend InputHandler;
	friend Controller;
	friend PieceTracker;

	// Helpers
	void loadBoard(std::string fileName);
	void saveBoard(std::string fileName);

public:
	// Constructors
	Game();
	~Game();

	// Methods
	void run();

	// Event handlers
	void onCameraChange();
	void onGeneration(MoveMarker* marker);
};

#endif // CHESS_GAME_H

