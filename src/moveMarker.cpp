#include "moveMarker.h"
#include <SFML/Graphics.hpp>

// Public constructors / destructor

/**
 * Constructor
 */
MoveMarker::MoveMarker(GamePiece* rootPiece_, PieceMove* rootMove, sf::Vector2i* baseVector_, sf::Vector2i pos_) :
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
	destroyChildren();

	// Get rid of previous references to this move marker
	if (prev != nullptr) {
        prev->next = nullptr;
        prev = nullptr;
	}

	rootPiece  = nullptr;
	rootMove   = nullptr;
	baseVector = nullptr;
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
	return pos - *baseVector;
}

/**
 * Iterate the move forward to get the position of the next move marker
 */
sf::Vector2i MoveMarker::getNextPos() const {
	return pos + *baseVector;
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



// Public utility methods

/**
 * Determine whether the move marker is at one of the ends of the move
 */
bool MoveMarker::isTerminal() {
	return (prev == nullptr) || (next == nullptr);
}

/**
 * Delete all the "next" move markers
 */
void MoveMarker::destroyChildren() {
	delete next;
	next = nullptr;
}
