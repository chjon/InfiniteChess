#include "moveMarker.h"
#include "pieceMove.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "pieceDef.h"
#include "piece.h"

// Public constructors / destructor

/**
 * Constructor
 */
MoveMarker::MoveMarker(Piece* rootPiece_, const PieceMove* rootMove, sf::Vector2i baseVector_, sf::Vector2i pos_) :
	rootPiece{rootPiece_},
	rootMove{rootMove},
	baseVector{baseVector_},
	pos{pos_},
	lambda{1}
{
}

/**
 * Destructor
 */
MoveMarker::~MoveMarker() {
	rootPiece  = nullptr;
	rootMove   = nullptr;
}



// Accessors

/**
 * Get the move marker's position
 */
sf::Vector2i MoveMarker::getPos() const {
	return pos;
}

/**
 * Iterate the move backward to get the position of the previous move marker
 */
sf::Vector2i MoveMarker::getPrevPos() const {
	return pos - baseVector;
}

/**
 * Iterate the move forward to get the position of the next move marker
 */
sf::Vector2i MoveMarker::getNextPos() const {
	return pos + baseVector;
}

/**
 * Determine whether the move marker is a valid move position
 */
bool MoveMarker::canMove() const {
	Piece* pieceAtLocation = rootMove->pieceTracker->getPiece(pos);

	/*/ Check whether the new position meets the attack requirements
	if (((pieceAtLocation != nullptr) && rootMove->moveType == PieceMove::MoveType::MOVE_ONLY) ||
		((pieceAtLocation != nullptr) && pieceAtLocation->team == rootPiece->team) ||
		((pieceAtLocation == nullptr) && rootMove->moveType == PieceMove::MoveType::ATTACK_ONLY)
	) {
		return false;
	}*/

	// Check whether there are pieces in the way of the move
	if (!rootMove->canLeap) {
		sf::Vector2i cur = rootPiece->getPos() + baseVector;

		while (cur != pos) {
			pieceAtLocation = rootMove->pieceTracker->getPiece(cur);

			if (pieceAtLocation != nullptr) {
				return false;
			}

			cur += baseVector;
		}
	}

	return true;
}
