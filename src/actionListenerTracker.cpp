#include "actionListenerTracker.h"

#include "moveMarker.h"
#include "piece.h"
#include "vectorUtils.h"

// Constructors
ActionListenerTracker::ActionListenerTracker() {
}

/**
 * Destructor
 */
ActionListenerTracker::~ActionListenerTracker() {
	for (std::map<
		sf::Vector2i,
		std::map<std::string, MoveMarker*>*,
		VectorUtils::cmpVectorLexicographically
	>::iterator i = actionListeners.begin();
		i != actionListeners.end(); ++i
	) {
        delete i->second;
	}

	actionListeners.clear();
}

// Helpers

/**
 * Add a move listener
 *
 * @param positionToNotify the position to listen to
 * @param listener the move marker to notify upon update of the trigger position
 */
void ActionListenerTracker::addListener(sf::Vector2i positionToNotify, MoveMarker* listener) {
    // Get the existing listeners for the trigger position
    std::map<
		sf::Vector2i,
		std::map<std::string, MoveMarker*>*,
		VectorUtils::cmpVectorLexicographically
	>::iterator triggerIter =
		actionListeners.find(positionToNotify);

	std::map<std::string, MoveMarker*>* positionListeners;

	// Check if there are existing listeners for the trigger position
    if (triggerIter == actionListeners.end()) {
        positionListeners = new std::map<std::string, MoveMarker*>();
        actionListeners.insert(std::make_pair(positionToNotify, positionListeners));
    } else {
		positionListeners = triggerIter->second;
    }

    // Add the listener
    std::string key = generateKey(listener);
    if (positionListeners->find(key) == positionListeners->end()) {
		positionListeners->insert(std::make_pair(key, listener));
    }
}

/**
 * Generate a key from a move marker
 */
std::string ActionListenerTracker::generateKey(MoveMarker* marker) {
	return VectorUtils::toString(marker->getRootPiece()->getPos());
}

// Public API

/**
 * Add a move listener
 *
 * @param marker the move marker to notify upon update of the trigger position
 */
void ActionListenerTracker::addListeners(MoveMarker* marker) {
    const std::vector<sf::Vector2i>* targetPositions = marker->getTargetedPositions();
	for (std::vector<sf::Vector2i>::const_iterator i = targetPositions->begin(); i != targetPositions->end(); ++i) {
		addListener(*i, marker);
	}

	// Clean up
	delete targetPositions;
}

/**
 * Remove all the action listeners for a given move marker
 *
 * @param listener the move marker to remove move listeners for
 */
void ActionListenerTracker::removeListeners(MoveMarker* listener) {
	// A list of all the positions to erase
	std::vector<sf::Vector2i> toDelete;

    // Iterate through all the tracked positions
    for (std::map<sf::Vector2i, std::map<std::string, MoveMarker*>*>::iterator i = actionListeners.begin();
		i != actionListeners.end(); ++i
	) {
		std::map<std::string, MoveMarker*>* positionListeners = i->second;
		std::map<std::string, MoveMarker*>::iterator j = positionListeners->begin();

        // Iterate through all the listeners for the positions
        while (j != positionListeners->end() && j->second != listener) {
			++j;
        }

        // Remove the listener if it is listening to the position
        if (j != positionListeners->end()) {
            positionListeners->erase(j);

            // Mark the list to be deleted if there are no more listeners at the position
            if (positionListeners->empty()) {
				toDelete.push_back(i->first);
            }
        }
    }

    // Delete all the lists that were marked to be deleted
    for (std::vector<sf::Vector2i>::iterator i = toDelete.begin();
		i != toDelete.end(); ++i
    ) {
		actionListeners.erase(*i);
    }
}

/**
 * Notify the action listeners that are listening to the given position
 *
 * @param positionToNotify the position to notify
 * @param event the action that triggered the notification
 */
void ActionListenerTracker::notify(sf::Vector2i positionToNotify, Event* event) {
	// Get the existing listeners for the trigger position
    std::map<
		sf::Vector2i,
		std::map<std::string, MoveMarker*>*,
		VectorUtils::cmpVectorLexicographically
	>::iterator triggerIter =
		actionListeners.find(positionToNotify);



	// Check if there are existing listeners for the trigger position
    if (triggerIter != actionListeners.end()) {
		std::map<std::string, MoveMarker*>* positionListeners = triggerIter->second;

		// Notify each of the listeners at the position
		for (std::map<std::string, MoveMarker*>::iterator i = positionListeners->begin();
			i != positionListeners->end(); ++i
		) {
            i->second->handleEvent(event);
		}
    }
}
