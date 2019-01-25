#include "actionListenerTracker.h"

#include "moveMarker.h"

// Constructors
ActionListenerTracker::ActionListenerTracker() {
}

/**
 * Destructor
 */
ActionListenerTracker::~ActionListenerTracker() {
	for (std::map<
		sf::Vector2i,
		std::vector<MoveMarker*>*,
		VectorUtils::cmpVectorLexicographically
	>::iterator i = actionListeners.begin();
		i != actionListeners.end(); ++i
	) {
        delete i->second;
	}

	actionListeners.clear();
}

// Private helpers

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
		std::vector<MoveMarker*>*,
		VectorUtils::cmpVectorLexicographically
	>::iterator triggerIter =
		actionListeners.find(positionToNotify);

	// Check if there are existing listeners for the trigger position
    if (triggerIter != actionListeners.end()) {
		std::vector<MoveMarker*>* positionListeners = triggerIter->second;

		// Notify each of the listeners at the position
		for (std::vector<MoveMarker*>::iterator i = positionListeners->begin();
			i != positionListeners->end(); ++i
		) {
            (*i)->handleEvent(event);
		}
    }
}



// Public API

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
		std::vector<MoveMarker*>*,
		VectorUtils::cmpVectorLexicographically
	>::iterator triggerIter =
		actionListeners.find(positionToNotify);

	std::vector<MoveMarker*>* positionListeners;

	// Check if there are existing listeners for the trigger position
    if (triggerIter == actionListeners.end()) {
        positionListeners = new std::vector<MoveMarker*>();
        actionListeners.insert(std::make_pair(positionToNotify, positionListeners));
    } else {
		positionListeners = triggerIter->second;
    }

    // Add the listener
    positionListeners->push_back(listener);
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
    for (std::map<sf::Vector2i, std::vector<MoveMarker*>*>::iterator i = actionListeners.begin();
		i != actionListeners.end(); ++i
	) {
		std::vector<MoveMarker*>* positionListeners = i->second;
		std::vector<MoveMarker*>::iterator j = positionListeners->begin();

        // Iterate through all the listeners for the positions
        while (j != positionListeners->end() && *j != listener) {
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
 * Add an event to the event queue
 *
 * @param positionToNotify the position for the event
 * @param event the event to add to the queue
 */
void ActionListenerTracker::queueEvent(sf::Vector2i positionToNotify, Event* event) {
    eventQueue.push_back(std::make_pair(positionToNotify, event));
}

/**
 * Notify the listeners for each queued event
 */
void ActionListenerTracker::notify() {
	// Iterate through each queued event
    for (std::vector<std::pair<sf::Vector2i, Event*>>::iterator i = eventQueue.begin();
		i != eventQueue.end(); ++i
	) {
        notify(i->first, i->second);
    }

    // Clear the event queue
    eventQueue.clear();
}
