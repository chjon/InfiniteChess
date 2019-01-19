#include "moveMarker.h"

#include <SFML/Graphics.hpp>
#include "moveDef.h"
#include "numRule.h"
#include "piece.h"
#include "pieceDef.h"

// Public constructors / destructor

/**
 * Constructor
 */
MoveMarker::MoveMarker(
	const Piece* rootPiece_, const MoveDef* rootMove, sf::Vector2i baseVector_,
	sf::Vector2i pos_, unsigned int lambda_
) :
	rootPiece{rootPiece_},
	rootMove{rootMove},
	baseVector{baseVector_},
	pos{pos_},
	lambda{lambda_},
	next{nullptr},
	prev{nullptr},
	requiresLeap{false}
{
}

/**
 * Constructor
 */
MoveMarker::MoveMarker(
	const Piece* rootPiece_, const MoveDef* rootMove, sf::Vector2i baseVector_, sf::Vector2i pos_) :
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
 * Get the move marker's scale factor
 */
unsigned int MoveMarker::getLambda() const {
	return lambda;
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
bool MoveMarker::canMove(PieceTracker* pieceTracker, const Piece* testPiece) const {
	const Piece* pieceAtLocation = (testPiece == nullptr) ? (pieceTracker->getPiece(pos)) : (testPiece);

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

	// Check whether the position meets the scaling rules
	bool meetsScalingRule = false;
	for (std::vector<NumRule*>::const_iterator i = rootMove->scalingRules->begin();
		i != rootMove->scalingRules->end(); ++i
	) {
		// Check if the position meets the scaling rule
        if ((*i)->matches(lambda)) {
			meetsScalingRule = true;
			break;
        }
	}

	if (!meetsScalingRule) {
		return false;
	}

	// Check whether the position meets the nth step rules
	bool meetsNthStepRule = false;
	for (std::vector<NumRule*>::const_iterator i = rootMove->nthStepRules->begin();
		i != rootMove->nthStepRules->end(); ++i
	) {
		// Check if the position meets the nth step rule
        if ((*i)->matches(rootPiece->getMoveCount())) {
			meetsNthStepRule = true;
			break;
        }
	}

	if (!meetsNthStepRule) {
		return false;
	}

	// Check if the attacked position contains a royal piece
	if (pieceAtLocation != nullptr && pieceAtLocation->getDef()->isRoyal) {
		return true;
	}

	// Check whether the position is under attack if the piece is check vulnerable
	if (rootPiece->getDef()->isCheckVulnerable) {
		std::vector<MoveMarker*>* markers = pieceTracker->getMoveMarkers(pos);

		bool positionIsAttacked = false;
		for (std::vector<MoveMarker*>::iterator i = markers->begin(); i != markers->end(); ++i) {
            if ((*i)->rootPiece->getTeam() != rootPiece->getTeam() &&
				(*i)->rootMove->attacksEnemies &&
				(*i)->canMove(pieceTracker, rootPiece)
			) {
				positionIsAttacked = true;
				break;
            }
		}

		delete markers;

		if (positionIsAttacked) {
			return false;
		}
	}

	return true;
}

bool MoveMarker::canMove(PieceTracker* pieceTracker) const {
	return canMove(pieceTracker, nullptr);
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
