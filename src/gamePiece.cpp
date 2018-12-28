#include "gamePiece.h"
#include "pieceMove.h"

// Public constructors

/**
 * Empty constructor
 */
GamePiece::GamePiece() {

}

/**
 * Constructor
 */
GamePiece::GamePiece(
	std::string name_,
	std::string team_,
	sf::Vector2i pos_,
	unsigned int moveCount_
) :
	name{name_},
	team{team_},
	pos{pos_},
	moveCount{moveCount_}
{
}

/**
 * Destructor
 */
GamePiece::~GamePiece() {
}



// Public mutators

void GamePiece::addMove(PieceMove newMove) {
    moveSet.push_back(newMove);
}



// Public methods

/**
 * Check whether the piece can move to the requested spot
 *
 * @param newPos the desired position
 */
bool GamePiece::canMove(sf::Vector2i newPos) {
	for (PieceMove move : moveSet) {
		if (move.canMove(this, newPos)) {
			return true;
		}
	}

	return false;
}

/**
 * Move the piece
 */
void GamePiece::move(sf::Vector2i newPos) {
	pos = newPos;
}
