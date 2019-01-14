#include "moveMarker.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include "moveDef.h"
#include "piece.h"
#include "pieceDef.h"

// Public constructors / destructor

/**
 * Constructor
 */
MoveMarker::MoveMarker(const Piece* rootPiece_, const MoveDef* rootMove, sf::Vector2i baseVector_, sf::Vector2i pos_) :
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
 * Iterate the move forward to get the position of the next move marker
 */
sf::Vector2i MoveMarker::getNextPos() const {
	return pos + baseVector;
}

/**
 * Get the root move for the move marker
 */
const MoveDef* MoveMarker::getRootMove() const {
	return rootMove;
}

/**
 * Get the base vector for the move marker
 */
const sf::Vector2i MoveMarker::getBaseVector() const {
	return baseVector;
}

/**
 * Determine whether the move marker is a valid move position
 */
bool MoveMarker::canMove() const {
	/*const Piece* pieceAtLocation = rootMove->pieceTracker->getPiece(pos);

	// Check whether the new position meets the attack requirements
	if (((pieceAtLocation != nullptr) && rootMove->moveType == PieceMove::MoveType::MOVE_ONLY) ||
		((pieceAtLocation != nullptr) && pieceAtLocation->team == rootPiece->team) ||
		((pieceAtLocation == nullptr) && rootMove->moveType == PieceMove::MoveType::ATTACK_ONLY)
	) {
		return false;
	}

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
	}*/

	return true;
}
