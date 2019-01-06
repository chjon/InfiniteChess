#include "moveMarker.h"
#include "pieceMove.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Public constructors / destructor

/**
 * Constructor
 */
MoveMarker::MoveMarker(GamePiece* rootPiece_, const PieceMove* rootMove, sf::Vector2i baseVector_, sf::Vector2i pos_) :
	rootPiece{rootPiece_},
	rootMove{rootMove},
	baseVector{baseVector_},
	prev{nullptr},
	next{nullptr},
	pos{pos_},
	lambda{1}
{
}

/**
 * Destructor
 */
MoveMarker::~MoveMarker() {
	next = nullptr;

	// Get rid of previous references to this move marker
	if (prev != nullptr) {
        prev->next = nullptr;
        prev = nullptr;
	}

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
 * Get the previous move marker
 */
MoveMarker* MoveMarker::getPrev() const {
	return prev;
}

/**
 * Get the next move marker
 */
MoveMarker* MoveMarker::getNext() const {
	return next;
}

/**
 * Determine whether the move marker is a valid move position
 */
bool MoveMarker::canMove() const {
    GamePiece* pieceAtLocation = rootMove->pieceTracker->getPiece(pos);

	// Check whether the new position meets the attack requirements
	if ((rootMove->moveType == PieceMove::MoveType::MOVE_ONLY   && (pieceAtLocation != nullptr)) ||
		(rootMove->moveType == PieceMove::MoveType::ATTACK_ONLY && (pieceAtLocation == nullptr))
	) {
		return false;
	}

	return true;
}



// Mutators

/**
 * Set the previous move marker
 */
void MoveMarker::setPrev(MoveMarker* newPrev) {
	// Unlink the previous move marker from this move marker
	if (prev != nullptr) {
		prev->next = nullptr;
	}

	// Link the new previous move marker to this move marker
    if (newPrev != nullptr) {
		// Unlink the new previous move marker's next move marker
		if (newPrev->next != nullptr) {
			newPrev->next->prev = nullptr;
		}

		newPrev->next = this;
    }

    // Link this move marker to the new previous move marker
    prev = newPrev;
}

/**
 * Set the next move marker
 */
void MoveMarker::setNext(MoveMarker* newNext) {
	// Unlink the next move marker from this move marker
	if (next != nullptr) {
		next->prev = nullptr;
	}

	// Link the new next move marker to this move marker
    if (newNext != nullptr) {
		// Unlink the new next move marker's previous move marker
		if (newNext->prev != nullptr) {
			newNext->prev->next = nullptr;
		}

		newNext->prev = this;
    }

    // Link this move marker to the new next move marker
    next = newNext;
}



// Public utility methods

/**
 * Determine whether the move marker is at one of the ends of the move
 */
bool MoveMarker::isTerminal() {
	return (prev == nullptr) || (next == nullptr);
}
