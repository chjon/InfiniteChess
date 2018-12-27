#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "renderer.h"

class Game {
private:
	Renderer* renderer;

public:
	Game();
	~Game();
	void run();
};

#endif // CHESS_GAME_H

