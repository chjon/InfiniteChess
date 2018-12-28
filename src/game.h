#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "renderer.h"

class Game {
private:
    sf::RenderWindow* window;
	Renderer* renderer;

	void tick();
	void checkKeyboard();
	void onKeyPress(sf::Event::KeyEvent keyEvent);

public:
	Game();
	~Game();
	void run();
};

#endif // CHESS_GAME_H

