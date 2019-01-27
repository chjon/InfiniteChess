#include "controller.h"

#include <iostream>
#include "actionListenerTracker.h"
#include "event.h"
#include "eventProcessor.h"
#include "moveMarker.h"
#include "piece.h"
#include "targetingRule.h"

// Private event handlers

void Controller::onStartup() {
	// Update all of the pieces in the piece tracker
	const std::vector<Piece*>* pieces = pieceTracker->getPieces();
	for (std::vector<Piece*>::const_iterator i = pieces->begin(); i != pieces->end(); ++i) {
		eventProcessor.insertInQueue(EventProcessor::AFTER, new Event(*i, "enter", ""));
	}

	eventProcessor.executeEvents();

	// Clean up
	delete pieces;
	pieces = nullptr;
}

/**
 * Select/deselect a square
 */
void Controller::onMousePress(sf::Vector2i pos) {
	// Select piece
    if (selectedPiece == nullptr) {
		selectedPiece = game->pieceTracker->getPiece(pos);

	// Deselect
    } else if (selectedPiece->getPos() == pos || !selectedPiece->canMove(pos)) {
    	selectedPiece = nullptr;

	// Move piece
	} else {
		move(pos);
	}

	game->renderer->needsRedraw = true;
}



// Private helpers

void Controller::move(sf::Vector2i pos) {
	// Set up events for moving the piece
	eventProcessor.insertInQueue(EventProcessor::START, new Event(selectedPiece, "leave", ""));
	eventProcessor.insertInQueue(EventProcessor::EVENT, new Event(selectedPiece, "move", VectorUtils::toString(pos)));
	eventProcessor.insertInQueue(EventProcessor::AFTER, new Event(selectedPiece, "enter", ""));

	// Get the targets for moving to the position
	const std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>* targets = selectedPiece->getTargets(pos);

	// Take the first valid target piece
	Piece* targetPiece = nullptr;
	Event* targetEvent = nullptr;

	for (std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>::const_iterator i = targets->begin();
		i != targets->end(); ++i
	) {
		MoveMarker* marker = std::get<0>(*i);
        if (marker->canMove()) {
			targetPiece = std::get<1>(*i);
			targetEvent = std::get<2>(*i)->getEvent();
			break;
        }
	}

	if (targetPiece != nullptr) {
		if ("move" == targetEvent->action) {
			eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "leave", ""));
			eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "move", targetEvent->args));
			eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "enter", ""));
		} else if ("destroy" == targetEvent->action && targetPiece != nullptr) {
			eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "leave", ""));
			eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "destroy", ""));
		}
	}

	// Remove the piece that is at the destination position
	Piece* destPiece = pieceTracker->getPiece(pos);
	if (destPiece != nullptr && destPiece != targetPiece) {
		eventProcessor.insertInQueue(EventProcessor::START, new Event(destPiece, "leave", ""));
		eventProcessor.insertInQueue(EventProcessor::EVENT, new Event(destPiece, "destroy", ""));
	}

	// Execute all the queued events
	eventProcessor.executeEvents();

	// Deselect the piece
	selectedPiece = nullptr;

	// Clean up
	delete targets;
	targets = nullptr;
}


// Public constructors

/**
 * Constructor
 */
Controller::Controller(Game* g, PieceTracker* p) :
	game{g},
	pieceTracker{p},
	eventProcessor{p, actionListenerTracker},
	selectedPiece{nullptr}
{
}

/**
 * Destructor
 */
Controller::~Controller() {
	selectedPiece = nullptr;
}



// Accessors

/**
 * Get the currently selected piece
 */
Piece* Controller::getSelectedPiece() const {
	return selectedPiece;
}
