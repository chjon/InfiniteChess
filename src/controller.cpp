#include "controller.h"

#include "actionListenerTracker.h"
#include "event.h"
#include "eventProcessor.h"
#include "moveDef.h"
#include "moveMarker.h"
#include "piece.h"
#include "targetingRule.h"

// Private event handlers

void Controller::onStartup(
	std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams_,
	unsigned int curTeam_
) {
	NUM_TEAMS = teams_->size();
	teams = new unsigned int[NUM_TEAMS];

	// Create a list of all the teams
	unsigned int index = 0;
	std::map<const unsigned int, std::pair<const std::string, sf::Color>>::iterator i = teams_->begin();
	while (i != teams_->end()) {
		// Save the team index
		teams[index] = i->first;

		// Get the index of the team to go first
        if (i->first == curTeam_) {
			curTurn = index;
        }

        ++i;
        ++index;
	}

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

		// Check whether the piece can be selected this turn
		if (selectedPiece != nullptr && teams[curTurn] != selectedPiece->getTeam()) {
			selectedPiece = nullptr;
		}

	// Deselect
    } else if (selectedPiece->getPos() == pos) {
    	selectedPiece = nullptr;

	// Move piece
	} else {
		const MoveMarker* dest = selectedPiece->getValidMove(pos);

		// Deselect
		if (dest == nullptr) {
			selectedPiece = nullptr;
		} else {
			move(dest);
		}
	}

	game->renderer->needsRedraw = true;
}



// Private helpers

void Controller::move(const MoveMarker* dest) {
	sf::Vector2i pos = dest->getPos();

	// Set up events for moving the piece
	eventProcessor.insertInQueue(EventProcessor::START, new Event(selectedPiece, "leave", ""));
	eventProcessor.insertInQueue(EventProcessor::EVENT, new Event(
		selectedPiece, "move", std::to_string(dest->getRootMove()->index) + "," + VectorUtils::toString(pos) + ","
	));
	eventProcessor.insertInQueue(EventProcessor::AFTER, new Event(selectedPiece, "enter", ""));

	// Remove the piece that is at the destination position
	Piece* destPiece = pieceTracker->getPiece(pos);
	if (destPiece != nullptr) {
		eventProcessor.insertInQueue(EventProcessor::START, new Event(destPiece, "leave", ""));
		eventProcessor.insertInQueue(EventProcessor::EVENT, new Event(destPiece, "destroy", ""));
	}

	// Get the targets for moving to the position
	const std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>* targets = selectedPiece->getTargets(pos);

	// Take all of the targets
	for (std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>::const_iterator i = targets->begin();
		i != targets->end(); ++i
	) {
		// Check whether the target is valid
		MoveMarker* marker = std::get<0>(*i);
        if (!marker->canMove()) continue;

		Piece* targetPiece = std::get<1>(*i);
		const std::vector<Event*>* targetEvents = std::get<2>(*i)->getEvents();

		// Check whether there is a target piece
		if (targetPiece == nullptr || targetPiece == destPiece) continue;

		// Handle the different actions
		for (std::vector<Event*>::const_iterator j = targetEvents->begin(); j != targetEvents->end(); ++j) {
			if ("move" == (*j)->action) {
				eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "leave", ""));
				sf::Vector2i targetVector = VectorUtils::fromString((*j)->args);
				targetVector = MoveDef::rotate(targetVector, dest->getRootPiece()->getDir());
				targetVector = VectorUtils::reflect(targetVector, dest->switchedX, dest->switchedY, dest->switchedXY);
				eventProcessor.insertInQueue(EventProcessor::START, new Event(
					targetPiece, "move", "-1," + VectorUtils::toString(targetVector + targetPiece->getPos()) + ","
				));
				eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "enter", ""));

			} else if ("destroy" == (*j)->action && targetPiece != nullptr) {
				eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "leave", ""));
				eventProcessor.insertInQueue(EventProcessor::START, new Event(targetPiece, "destroy", ""));
			}
		}
	}

	// Execute all the queued events
	eventProcessor.executeEvents();

	// Progress to the next turn
	if (dest->getRootMove()->endsTurn) {
		curTurn = (curTurn + 1) % NUM_TEAMS;
	}

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
	teams{nullptr},
	curTurn{0},
	selectedPiece{nullptr}
{
}

/**
 * Destructor
 */
Controller::~Controller() {
	delete[] teams;
	teams = nullptr;
	selectedPiece = nullptr;
}



// Accessors

/**
 * Get the currently selected piece
 */
Piece* Controller::getSelectedPiece() const {
	return selectedPiece;
}

/**
 * Determine whether it is this team's turn to move
 */
bool Controller::canMove(unsigned int team) const {
	return teams[curTurn] == team;
}
