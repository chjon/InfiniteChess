#include "controller.h"

#include "actionListenerTracker.h"
#include "event.h"
#include "piece.h"

// Private event handlers

/**
 * Select/deselect a square
 */
void Controller::onMousePress(sf::Vector2i pos) {
	// Select piece
    if (selectedPiece == nullptr) {
		selectedPiece = game->pieceTracker->getPiece(pos);

	// Deselect
    } else if (selectedPiece->getPos() == pos) {
    	selectedPiece = nullptr;

	// Move piece
	} else if (selectedPiece->canMove(pos)) {
		actionListenerTracker.queueEvent(selectedPiece->getPos(), new Event(selectedPiece, "leave"));

		/*MoveMarker* dest = pieceTracker->getValidMove(selectedPiece, pos);

		if (dest != nullptr) {
			movePiece(dest);
		}*/
	}

	game->renderer->needsRedraw = true;
}



// Private methods

/**
 * Move the selected piece to a new position
 */
void Controller::movePiece(MoveMarker* dest) {
	// Move the piece at the position
	pieceTracker->movePiece(dest);

	// Deselect the piece
	selectedPiece = nullptr;
}



// Public constructors

/**
 * Constructor
 */
Controller::Controller(Game* g, PieceTracker* p) :
	game{g},
	pieceTracker{p},
	selectedPiece{nullptr}
{
}

/**
 * Destructor
 */
Controller::~Controller() {
	selectedPiece = nullptr;
}



// Accessors

/**
 * Get the currently selected piece
 */
Piece* Controller::getSelectedPiece() const {
	return selectedPiece;
}
