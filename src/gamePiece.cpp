#include "gamePiece.h"
#include "pieceMove.h"

// Private utility methods

/**
 * Do a full delete for the definition
 */
void GamePiece::definitionDelete() {
    // Delete each of the moves in the move set
    for (std::vector<PieceMove*>::const_iterator it = moveSet->begin(); it != moveSet->end(); it++) {
        delete *it;
    }

    delete moveSet;
    moveSet = nullptr;
}


// Public constructors

/**
 * Constructor for creating template pieces
 */
GamePiece::GamePiece(const std::string n, const std::vector<PieceMove*>* m) :
	name{n},
	moveSet{m}
{
}

/**
 * "Copy" constructor for copying from the definition
 */
GamePiece::GamePiece(const GamePiece* piece, const sf::Color team_, sf::Vector2i pos_) :
	name{piece->name},
	moveSet{piece->moveSet},
	team{team_},
	pos{pos_}
{
}

/**
 * Destructor
 */
GamePiece::~GamePiece() {
	moveSet = nullptr;
}



// Public methods

/**
 * Check whether the piece can move to the requested spot
 *
 * @param newPos the desired position
 */
bool GamePiece::canMove(const sf::Vector2i newPos) {
	for (PieceMove* move : *moveSet) {
		if (move->canMove(this, newPos)) {
			return true;
		}
	}

	return false;
}

/**
 * Move the piece
 */
void GamePiece::move(const sf::Vector2i newPos) {
	pos = newPos;
}
