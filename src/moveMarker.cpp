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
	lambda{1},
	next{nullptr},
	prev{nullptr},
	requiresLeap{false}
{
}

/**
 * Destructor
 */
MoveMarker::~MoveMarker() {
	rootPiece  = nullptr;
	rootMove   = nullptr;
	next       = nullptr;
	prev       = nullptr;
}



// Event handlers

/**
 * Update whether the move marker requires leaping when a piece leaves the tile
 */
void MoveMarker::onPieceLeave(PieceTracker* pieceTracker) {
	requiresLeap = false;

	// Check whether to propagate the event
	if (next != nullptr && pieceTracker->getPiece(pos) == nullptr) {
		next->onPieceLeave(pieceTracker);
	}
}



/**
 * Update whether the move marker requires leaping when a piece enters the tile
 */
void MoveMarker::onPieceEnter() {
	// Check whether the tile is already accessible only via leap
	if (!requiresLeap) {
		requiresLeap = true;

		// Check if there is a next tile
		if (next != nullptr) {
			next->onPieceEnter();
		}
	}
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
 * Get the next move marker
 */
MoveMarker* MoveMarker::getNext() const {
	return next;
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
 * Get whether the move marker requires a leap
 */
const bool MoveMarker::getRequiresLeap() const {
	return requiresLeap;
}

/**
 * Determine whether the move marker is a valid move position
 */
bool MoveMarker::canMove(PieceTracker* pieceTracker) const {
	const Piece* pieceAtLocation = pieceTracker->getPiece(pos);

	// Check whether the new position meets the attack requirements
	if (pieceAtLocation == nullptr) {
		if (!rootMove->movesEmpty) return false;
	} else if (pieceAtLocation->getTeam() == rootPiece->getTeam()) {
		if (!rootMove->attacksFriendlies) return false;
	} else {
		if (!rootMove->attacksEnemies) return false;
	}

	// Check whether there are pieces in the way of the move
	if (!rootMove->canLeap && requiresLeap) {
		return false;
	}

	return true;
}

// Mutators

/**
 * Set the next move marker
 */
void MoveMarker::setNext(MoveMarker* newNext) {
    next = newNext;

    // Link backward as well
    if (newNext != nullptr) {
		newNext->prev = this;
    }
}

/**
 * Set whether the move marker requires a leap
 */
void MoveMarker::setRequiresLeap(bool requiresLeap_) {
	requiresLeap = requiresLeap_;
}
