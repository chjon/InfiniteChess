#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "renderer.h"
#include "inputHandler.h"

class Game {
private:
	// Members
    sf::RenderWindow* window;
	Renderer* renderer;
	InputHandler* inputHandler;

public:
	// Constructors
	Game();
	~Game();

	// Methods
	void run();
};

#endif // CHESS_GAME_H

