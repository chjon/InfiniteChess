#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "game.h"

// Forward declarations
class Game;
class PieceTracker;
class Piece;
class InputHandler;
class MoveMarker;



// Class declaration
class Controller {
private:
	// Members
	Game* game;
	PieceTracker* pieceTracker;

	Piece* selectedPiece;

	// Event handlers
	void onMousePress (sf::Vector2i pos);

	// Methods
	void movePiece (MoveMarker* dest);

	// Friends
	friend InputHandler;

public:
	// Constructors
	Controller(Game* g, PieceTracker* p);
	~Controller();

	// Accessors
	Piece* getSelectedPiece() const;
};

#endif // CHESS_CONTROLLER_H
