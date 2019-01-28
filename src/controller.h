#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "actionListenerTracker.h"
#include "eventProcessor.h"
#include "game.h"

// Forward declarations
class ActionListenerTracker;
class Game;
class InputHandler;
class MoveMarker;
class PieceTracker;
class Piece;



// Class declaration
class Controller {
private:
	// Members
	Game* game;
	PieceTracker* pieceTracker;
	ActionListenerTracker actionListenerTracker;
	EventProcessor eventProcessor;

	Piece* selectedPiece;

	// Event handlers
	void onMousePress (sf::Vector2i pos);

	// Friends
	friend InputHandler;

	// Helpers
	void move(const MoveMarker* dest);

public:
	// Constructors
	Controller(Game* g, PieceTracker* p);
	~Controller();

	// Event handlers
	void onStartup();

	// Accessors
	Piece* getSelectedPiece() const;
};

#endif // CHESS_CONTROLLER_H
