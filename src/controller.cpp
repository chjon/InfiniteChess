#include "controller.h"

#include "component_trackers/actionListenerTracker.h"
#include "components/event.h"
#include "component_trackers/eventProcessor.h"
#include "components/moveDef.h"
#include "components/moveMarker.h"
#include "components/piece.h"
#include "components/targetingRule.h"

// Private event handlers

void Controller::onStartup(
	std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams_,
	unsigned int curTeam_
) {
	clearTeams();
	eventProcessor.onStartup();
	deselect();

	TeamNode* head = nullptr;
	curTurn = nullptr;

	// Store all the teams
	std::map<const unsigned int, std::pair<const std::string, sf::Color>>::iterator i = teams_->begin();
	while (i != teams_->end()) {
		// Create the team
		TeamNode* temp = new TeamNode{ i->first, nullptr, 0, i->second.first, i->second.second, false };
		teams.insert(std::make_pair(i->first, temp));
		if (head == nullptr) {
			head = temp;
		} else {
			curTurn->next = temp;
		}

		// Advance to the next team
		curTurn = temp;
        ++i;
	}

	// Link the teams in a loop
	curTurn->next = head;
	curTurn = teams.find(curTeam_)->second;

	// Update all of the pieces in the piece tracker
	const std::vector<Piece*>* pieces = pieceTracker->getPieces();
	for (std::vector<Piece*>::const_iterator i = pieces->begin(); i != pieces->end(); ++i) {
		eventProcessor.insertInQueue(EventProcessor::AFTER, new Event(*i, "enter", "initialization"));
		teams.find((*i)->getTeam())->second->numPieces++;
	}

	eventProcessor.executeEvents();

	// Clean up
	delete pieces;
	pieces = nullptr;
}

/**
 * Add a move marker as a listener when it is generated
 */
void Controller::onGeneration(MoveMarker* marker) {
	actionListenerTracker.addListeners(marker);
}

/**
 * Select/deselect a square
 */
void Controller::onMousePress(sf::Vector2i pos) {
	// Select piece
    if (selectedPiece == nullptr) {
		selectedPiece = game->pieceTracker->getPiece(pos);

		// Check whether the piece can be selected this turn
		if (selectedPiece != nullptr && curTurn->teamIndex != selectedPiece->getTeam()) {
			deselect();
		}

	// Deselect
    } else if (selectedPiece->getPos() == pos) {
    	deselect();

	// Move piece
	} else {
		const MoveMarker* dest = selectedPiece->getValidMove(pos, curTeamHasMoved());

		// Deselect
		if (dest == nullptr) {
			deselect();
		} else {
			move(dest);
		}
	}

	game->renderer->needsRedraw = true;
}



// Private helpers

void Controller::clearTeams() {
	// Delete the teams
	for (std::map<unsigned int, TeamNode*>::iterator i = teams.begin(); i != teams.end(); ++i) {
        delete i->second;
	}
	teams.clear();
	curTurn = nullptr;
}

void Controller::deselect() {
	selectedPiece = nullptr;

	// Progress to the next turn
	if (curTurn != nullptr && curTurn->moved) {
		advanceTurn();
	}
}

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
        if (!marker->canMove(curTeamHasMoved())) continue;

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

	bool endsTurn = dest->getRootMove()->endsTurn;
	curTurn->moved = true;

	// Execute all the queued events
	eventProcessor.executeEvents();

	if (endsTurn) {
		// Deselect the piece
		deselect();
	}

	// Clean up
	delete targets;
	targets = nullptr;
}

/**
 * Advance to the next turn
 */
void Controller::advanceTurn() {
	curTurn->moved = false;

	do {
		curTurn = curTurn->next;
	} while (curTurn->numPieces == 0);
}

// Public constructors

/**
 * Constructor
 */
Controller::Controller(Game* g, PieceTracker* p) :
	game{g},
	pieceTracker{p},
	eventProcessor{p, actionListenerTracker, this},
	curTurn{nullptr},
	selectedPiece{nullptr}
{
}

/**
 * Destructor
 */
Controller::~Controller() {
	clearTeams();
	curTurn = nullptr;

	deselect();
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
	return curTurn->teamIndex == team;
}
