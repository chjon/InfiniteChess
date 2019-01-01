#include "controller.h"

// Private event handlers

/**
 * Select/deselect a square
 */
void Controller::onMousePress(sf::Vector2i pos) {
	// Select piece
    if (selectedPiece == nullptr) {
		selectedPiece = game->pieceTracker->getPiece(pos);

	// Deselect
    } else if (selectedPiece->pos == pos) {
    	selectedPiece = nullptr;

	// Move piece
	} else if (selectedPiece->canMove(pos)) {
		movePiece(pos);
	}

	game->renderer->needsRedraw = true;
}



// Private methods

/**
 * Move the selected piece to a new position
 */
void Controller::movePiece(sf::Vector2i pos) {
	// Move the piece at the position
	game->pieceTracker->movePiece(selectedPiece->pos, pos);

	// Move the piece
	selectedPiece->move(pos);

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
GamePiece* Controller::getSelectedPiece() const {
	return selectedPiece;
}