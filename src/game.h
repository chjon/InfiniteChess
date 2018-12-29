#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <SFML/Graphics.hpp>
#include "renderer.h"
#include "inputHandler.h"
#include "pieceTracker.h"

// Forward declarations
class Renderer;
class InputHandler;
class PieceTracker;



// Class declaration
class Game {
private:
	// Members
    sf::RenderWindow* window;
	Renderer* renderer;
	InputHandler* inputHandler;
	PieceTracker* pieceTracker;

	// Friends
	friend Renderer;
	friend InputHandler;

public:
	// Constructors
	Game();
	~Game();

	// Methods
	void run();
};

#endif // CHESS_GAME_H

