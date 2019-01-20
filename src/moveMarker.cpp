#include "moveMarker.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include "moveDef.h"
#include "numRule.h"
#include "piece.h"
#include "pieceDef.h"
#include "pieceTracker.h"
#include "targetingRule.h"

// Private helpers

/**
 * Determine whether a piece meets the attack requirements
 */
void MoveMarker::updateMeetsAttackRequirements(PieceTracker* pieceTracker) {
	// Check whether the new position meets the attack requirements
	Piece* piece = pieceTracker->getPiece(pos);
	meetsAttackRequirements = true;
	if (piece == nullptr) {
		if (!rootMove->movesEmpty) meetsAttackRequirements = false;
	} else if (piece->getTeam() == rootPiece->getTeam()) {
		if (!rootMove->attacksFriendlies) meetsAttackRequirements = false;
	} else {
		if (!rootMove->attacksEnemies) meetsAttackRequirements = false;
	}
}

/**
 * Determine whether the move marker meets a NumRule
 */
bool MoveMarker::meetsNumRule(const std::vector<NumRule*>* numRules, unsigned int candidate, bool deleteList) const {
	// Check if the candidate meets any of the num rules
	bool meetsNumRule = false;
	for (std::vector<NumRule*>::const_iterator i = numRules->begin(); i != numRules->end(); ++i) {
        if ((*i)->matches(candidate)) {
			meetsNumRule = true;
        }
	}

	// Delete the input list if required
	if (deleteList) {
		delete numRules;
	}

	return meetsNumRule;
}

/**
 * Determine whether the move marker meets a targeting rule
 */
bool MoveMarker::meetsTargetingRule(PieceTracker* pieceTracker) const {
    for (std::vector<const TargetingRule*>::const_iterator i = rootMove->targetingRules->begin();
		i != rootMove->targetingRules->end(); ++i
	) {
        // Get position identified by targeting rule
        const sf::Vector2i rotated = MoveDef::rotate((*i)->offsetVector, rootPiece->getDir());
        const sf::Vector2i transformed = MoveDef::reflect(rotated, switchedX, switchedY, switchedXY);

        // Check if the targeting rule is met
        if ((*i)->matches(rootPiece, pieceTracker->getPiece(pos + transformed))) {
			return true;
        }
    }

    return false;
}

/**
 * Determine whether the position is being attacked
 */
bool MoveMarker::isAttacked(PieceTracker* pieceTracker) const {
	std::vector<MoveMarker*>* markers = pieceTracker->getMoveMarkers(pos);

	bool positionIsAttacked = false;
	for (std::vector<MoveMarker*>::iterator i = markers->begin(); i != markers->end(); ++i) {
		if (((*i)->rootPiece->getTeam() == rootPiece->getTeam()) ||
			(!(*i)->rootMove->attacksEnemies) ||
			(!(*i)->rootMove->canLeap && (*i)->requiresLeap) ||
			(!(*i)->meetsScalingRule) ||
			(!(*i)->meetsNthStepRule)
		) {
			continue;
		}

		positionIsAttacked = true;
		break;
	}

	delete markers;
	return positionIsAttacked;
}


// Public constructors / destructor

/**
 * Constructor
 */
MoveMarker::MoveMarker(
	const Piece* rootPiece_, const MoveDef* rootMove, sf::Vector2i baseVector_, sf::Vector2i pos_,
	bool switchedX_, bool switchedY_, bool switchedXY_, unsigned int lambda_
) :
	rootPiece{rootPiece_},
	rootMove{rootMove},
	baseVector{baseVector_},
	pos{pos_},
	next{nullptr},
	prev{nullptr},
	requiresLeap{false},
	meetsAttackRequirements{false},
	meetsScalingRule{false},
	meetsNthStepRule{false},
	switchedX{switchedX_},
	switchedY{switchedY_},
	switchedXY{switchedXY_},
	lambda{lambda_}
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
 * Update the move marker when it is generated
 */
void MoveMarker::onGeneration(PieceTracker* pieceTracker) {
    updateMeetsAttackRequirements(pieceTracker);
    meetsScalingRule = meetsNumRule(rootMove->scalingRules, lambda, false);
    meetsNthStepRule = meetsNumRule(rootMove->nthStepRules, rootPiece->getMoveCount(), false);
}

/**
 * Update the move marker chain when a piece leaves the tile
 */
void MoveMarker::onPieceLeaveNext(Piece* piece, PieceTracker* pieceTracker) {
	requiresLeap = false;

	// Check whether to propagate the event
	if (next != nullptr && pieceTracker->getPiece(pos) == nullptr) {
		next->onPieceLeaveNext(piece, pieceTracker);
	}
}

/**
 * Update the move marker chain when a piece enters the tile
 */
void MoveMarker::onPieceEnterNext(Piece* piece, PieceTracker* pieceTracker) {
	// Check whether the tile is already accessible only via leap
	if (!requiresLeap) {
		requiresLeap = true;

		// Check if there is a next tile
		if (next != nullptr) {
			next->onPieceEnterNext(piece, pieceTracker);
		}
	}
}

/**
 * Update the move marker when a piece leaves the tile
 */
void MoveMarker::onPieceLeave(Piece* piece, PieceTracker* pieceTracker) {
	updateMeetsAttackRequirements(pieceTracker);

	if (!requiresLeap && next != nullptr) {
		next->onPieceLeaveNext(piece, pieceTracker);
	}
}

/**
 * Update the move marker when a piece enters the tile
 */
void MoveMarker::onPieceEnter(Piece* piece, PieceTracker* pieceTracker) {
	updateMeetsAttackRequirements(pieceTracker);

	if (next != nullptr) {
		next->onPieceEnterNext(piece, pieceTracker);
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
	// Check if the position meets the movement requirements
	if ((!meetsAttackRequirements) ||
		(!rootMove->canLeap && requiresLeap) ||
		(!meetsScalingRule) ||
		(!meetsNthStepRule) ||
		(!meetsTargetingRule(pieceTracker)) ||
		(rootPiece->getDef()->isCheckVulnerable && isAttacked(pieceTracker))
	) {
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
