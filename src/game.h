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

	// Friends
	friend Renderer;
	friend InputHandler;
	friend Controller;
	friend PieceTracker;

public:
	// Constructors
	Game();
	~Game();

	// Methods
	void run();
};

#endif // CHESS_GAME_H

