#include "controller.h"

// Private event handlers

/**
 * Select/deselect a square
 */
void Controller::onMousePress(sf::Vector2f posF) {
	sf::Vector2i pos(std::floor(posF.x), std::floor(posF.y));

	// Select piece
    if (selectedPiece == nullptr) {
		selectedPiece = game->pieceTracker->getPiece(pos);

	// Deselect
    } else if (selectedPiece->pos == pos) {
    	selectedPiece = nullptr;

	// Move piece
	} else if (selectedPiece->canMove(pos)) {
		selectedPiece->move(pos);
    }

    game->renderer->needsRedraw = true;
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
