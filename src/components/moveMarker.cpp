#include "moveMarker.h"

#include <SFML/Graphics.hpp>
#include <tuple>
#include "event.h"
#include "moveDef.h"
#include "numRule.h"
#include "piece.h"
#include "pieceDef.h"
#include "../component_trackers/actionListenerTracker.h"
#include "targetingRule.h"

// Private helpers

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
 * Determine whether the move marker meets all of its targeting rules
 */
const bool MoveMarker::meetsTargetingRules() const {
    for (std::map<sf::Vector2i, std::tuple<bool, Piece*, const TargetingRule*>>::const_iterator i = targets->begin(); i != targets->end(); ++i) {
        if (!std::get<0>(i->second)) return false;
    }

    return true;
}

/**
 * Determine whether the position is being attacked
 */
bool MoveMarker::isAttacked(PieceTracker* pieceTracker) const {
	std::vector<MoveMarker*>* markers = pieceTracker->getMoveMarkers(pos);

	bool positionIsAttacked = false;
	for (std::vector<MoveMarker*>::iterator i = markers->begin(); i != markers->end(); ++i) {
		if (((*i)->rootPiece->getTeam() == rootPiece->getTeam()) ||
			(!(*i)->rootMove->canLeap && (*i)->numObstructions) ||
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
	const Piece* rootPiece_, const MoveDef* rootMove_, sf::Vector2i baseVector_, sf::Vector2i pos_,
	bool switchedX_, bool switchedY_, bool switchedXY_, unsigned int lambda_
) :
	rootPiece{rootPiece_},
	rootMove{rootMove_},
	baseVector{baseVector_},
	pos{pos_},
	next{nullptr},
	prev{nullptr},
	meetsScalingRule{false},
	meetsNthStepRule{false},
	numObstructions{0},
	targets{new std::map<sf::Vector2i, std::tuple<bool, Piece*, const TargetingRule*>, VectorUtils::cmpVectorLexicographically>()},
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
 * Handle an event
 *
 * @param event the event to handle
 */
void MoveMarker::handleEvent(Event* event) {
	Piece* piece = event->piece;

	sf::Vector2i pos = piece->getPos();

	std::map<
		sf::Vector2i,
		std::tuple<bool, Piece*, const TargetingRule*>,
		VectorUtils::cmpVectorLexicographically
	>::iterator ruleIter = targets->find(pos);

	// Check if the marker has a handler
	if (ruleIter == targets->end()) {
		return;
	}

	const TargetingRule* rule = std::get<2>(ruleIter->second);

    if ("leave" == event->action) {
        std::get<0>(ruleIter->second) = rule->matches(rootPiece, nullptr);
		std::get<1>(ruleIter->second) = nullptr;
    } else if ("enter" == event->action) {
		std::get<0>(ruleIter->second) = rule->matches(rootPiece, piece);
		std::get<1>(ruleIter->second) = piece;
    }
}

/**
 * Update the move marker when it is generated
 */
void MoveMarker::onGeneration(PieceTracker* pieceTracker) {
    meetsScalingRule = meetsNumRule(rootMove->scalingRules, lambda, false);
    meetsNthStepRule = meetsNumRule(rootMove->nthStepRules, rootPiece->getMoveCount(), false);

    // Generate map for targeting rules
	for (std::vector<const TargetingRule*>::const_iterator i = rootMove->targetingRules->begin();
		i != rootMove->targetingRules->end(); ++i
	) {
        // Get position identified by targeting rule
        const sf::Vector2i rotated = MoveDef::rotate((*i)->offsetVector, rootPiece->getDir());
        const sf::Vector2i transformed = VectorUtils::reflect(rotated, switchedX, switchedY, switchedXY);
		Piece* targetPiece = pieceTracker->getPiece(pos + transformed);

		targets->insert(std::make_pair(
			pos + transformed,
			std::make_tuple((*i)->matches(rootPiece, targetPiece), targetPiece, *i)
		));
    }
}

/**
 * Update the move marker chain's properties
 */
void MoveMarker::update(Piece* piece, PieceTracker* pieceTracker, unsigned int numObstructions_) {
	numObstructions = numObstructions_;

	// Check if there is a next tile
	if (next != nullptr) {
		next->update(piece, pieceTracker, numObstructions_ + (pieceTracker->getPiece(pos) != nullptr));
	}
}

/**
 * Update the move marker when a piece leaves the tile
 */
void MoveMarker::onPieceLeave(Piece* piece, PieceTracker* pieceTracker) {
	if (next != nullptr) {
		next->update(piece, pieceTracker, next->numObstructions - 1);
	}
}

/**
 * Update the move marker when a piece enters the tile
 */
void MoveMarker::onPieceEnter(Piece* piece, PieceTracker* pieceTracker) {
	if (next != nullptr) {
		next->update(piece, pieceTracker, next->numObstructions + 1);
	}
}



// Accessors

/**
 * Iterate the move forward to get the position of the next move marker
 */
sf::Vector2i MoveMarker::getNextPos() const {
	if (rootMove->constantMultiple && lambda >= rootMove->constantMultiple) {
		return pos;
	}

	return pos + baseVector;
}

/**
 * Get a list of all the move marker's targets
 */
const std::vector<std::pair<Piece*, const TargetingRule*>>* MoveMarker::getTargets() const {
	std::vector<std::pair<Piece*, const TargetingRule*>>* targetList =
		new std::vector<std::pair<Piece*, const TargetingRule*>>();

	for (std::map<
			sf::Vector2i,
			std::tuple<bool, Piece*, const TargetingRule*>,
			VectorUtils::cmpVectorLexicographically
		>::iterator i = targets->begin(); i != targets->end(); ++i
	) {
        if (std::get<0>(i->second)) {
			targetList->push_back(std::make_pair(std::get<1>(i->second), std::get<2>(i->second)));
        }
	}

	return targetList;
}

/**
 * Determine whether the move marker is a valid move position
 */
bool MoveMarker::canMove() const {
	// Check if the position meets the movement requirements
	if ((!rootMove->canLeap && numObstructions) ||
		(!meetsScalingRule) ||
		(!meetsNthStepRule) ||
		(!meetsTargetingRules())
		//(rootPiece->getDef()->isCheckVulnerable && isAttacked(pieceTracker))
	) {
		return false;
	}

	return true;
}

/**
 * Get a list of all the potential target positions that the move marker is tracking
 */
const std::vector<sf::Vector2i>* MoveMarker::getTargetedPositions() const {
	// Iterate through each targeting rule for the move marker
	std::vector<sf::Vector2i>* targetPositions = new std::vector<sf::Vector2i>();
	for (std::vector<const TargetingRule*>::const_iterator i = rootMove->targetingRules->begin();
		i != rootMove->targetingRules->end(); ++i
	) {
        // Get position identified by targeting rule
        const sf::Vector2i rotated = MoveDef::rotate((*i)->offsetVector, rootPiece->getDir());
        const sf::Vector2i transformed = VectorUtils::reflect(rotated, switchedX, switchedY, switchedXY);

		targetPositions->push_back(pos + transformed);
    }

    return targetPositions;
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
void MoveMarker::setNumObstructions(unsigned int numObstructions_) {
	numObstructions = numObstructions_;
}
