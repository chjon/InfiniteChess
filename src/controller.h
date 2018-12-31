#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "game.h"

// Forward declarations
class Game;
class PieceTracker;
class GamePiece;
class InputHandler;



// Class declaration
class Controller {
private:
	// Members
	Game* game;
	PieceTracker* pieceTracker;

	GamePiece* selectedPiece;

	// Event handlers
	void onMousePress (sf::Vector2i pos);

	// Methods
	void movePiece (sf::Vector2i pos);

	// Friends
	friend InputHandler;

public:
	// Constructors
	Controller(Game* g, PieceTracker* p);
	~Controller();

	// Accessors
	GamePiece* getSelectedPiece() const;
};

#endif // CHESS_CONTROLLER_H
