#include "eventProcessor.h"

#include <SFML/Graphics.hpp>
#include "actionListenerTracker.h"
#include "event.h"
#include "moveMarker.h"
#include "moveTracker.h"
#include "piece.h"
#include "pieceTracker.h"
#include "vectorUtils.h"

// Constructors
EventProcessor::EventProcessor(PieceTracker* pieceTracker_, ActionListenerTracker& actionListenerTracker_):
	pieceTracker{pieceTracker_},
	actionListenerTracker{actionListenerTracker_}
{
}

EventProcessor::~EventProcessor() {
	// Delete each event queue
	for (std::vector<std::vector<Event*>*>::const_iterator i = eventQueues.begin(); i != eventQueues.end(); ++i) {
		// Delete each stored event
        for (std::vector<Event*>::iterator j = (*i)->begin(); j != (*i)->end(); ++j) {
            delete *j;
        }

        (*i)->clear();
        delete *i;
	}
}



// Helpers

/**
 * Execute an event
 */
void EventProcessor::execute(Event* event) {
	Piece* piece = event->piece;

	if ("enter" == event->action) {
		// Update the piece
        piece->onMove();
        piece->onCameraChange(pieceTracker);

        // Update the other move markers
		std::vector<MoveMarker*>* enterPos = pieceTracker->getMoveMarkers(piece->getPos());
		for (std::vector<MoveMarker*>::iterator i = enterPos->begin(); i != enterPos->end(); ++i) {
			(*i)->onPieceEnter(piece, pieceTracker);
		}

		// Alert action listeners
        actionListenerTracker.notify(piece->getPos(), event);

		// Clean up
		delete enterPos;

	} else if ("leave" == event->action) {
		// Unregister action listeners
        const std::vector<MoveMarker*>* moveMarkers = piece->getMoveTracker()->getMoveMarkers();
        for (std::vector<MoveMarker*>::const_iterator i = moveMarkers->begin(); i != moveMarkers->end(); ++i) {
			actionListenerTracker.removeListeners(*i);
        }

		// Update the piece
        pieceTracker->removePiece(piece->getPos());

        // Update the other move markers
		std::vector<MoveMarker*>* leavePos = pieceTracker->getMoveMarkers(piece->getPos());
		for (std::vector<MoveMarker*>::iterator i = leavePos->begin(); i != leavePos->end(); ++i) {
			(*i)->onPieceLeave(piece, pieceTracker);
		}

		// Alert action listeners
		actionListenerTracker.notify(piece->getPos(), event);

		// Clean up
		delete leavePos;

	} else if ("move" == event->action) {
		// Get the arguments
		std::vector<std::string>* args = StringUtils::getList(event->args, ',', '[', ']');

		// Move the piece
        sf::Vector2i destPos = VectorUtils::fromString((*args)[1]);
        piece->setPos(destPos);
        piece->setLastMove(std::stoi((*args)[0]));
        pieceTracker->addPiece(piece);

	} else if ("destroy" == event->action) {
		// Delete the piece
		delete piece;
	}

	delete event;
}



// Public API

/**
 * Add an event to an event queue
 *
 * @param queueIndex the index of the queue to add to
 * @param event the event to add to the queue
 */
void EventProcessor::insertInQueue(int queueIndex, Event* event) {
    eventQueues[queueIndex]->push_back(event);
}

/**
 * Execute all of the events
 */
void EventProcessor::executeEvents() {
	// Execute the events in each event queue
	for (std::vector<std::vector<Event*>*>::const_iterator i = eventQueues.begin(); i != eventQueues.end(); ++i) {
		// Delete each stored event
        for (std::vector<Event*>::iterator j = (*i)->begin(); j != (*i)->end(); ++j) {
            execute(*j);
        }

        (*i)->clear();
	}
}
