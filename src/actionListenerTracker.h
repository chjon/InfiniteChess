#ifndef CHESS_ACTION_LISTENER_TRACKER_H
#define CHESS_ACTION_LISTENER_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "vectorUtils.h"

// Forward declarations
class Event;
class MoveMarker;

// Class declaration
class ActionListenerTracker {
private:
    std::map<sf::Vector2i, std::vector<MoveMarker*>*, VectorUtils::cmpVectorLexicographically> actionListeners;
    std::vector<std::pair<sf::Vector2i, Event*>> eventQueue;

    // Helpers

    /**
     * Notify the action listeners that are listening to the given position
     *
     * @param positionToNotify the position to notify
     * @param event the action that triggered the notification
     */
    void notify(sf::Vector2i positionToNotify, Event* event);

public:
    // Constructors
    ActionListenerTracker();
	~ActionListenerTracker();

	// Public API

	/**
	 * Add a move listener
	 *
	 * @param positionToNotify the position to listen to
	 * @param listener the move marker to notify upon update of the trigger position
	 */
    void addListener(sf::Vector2i positionToNotify, MoveMarker* listener);

    /**
     * Remove all the action listeners for a given move marker
     *
     * @param listener the move marker to remove move listeners for
     */
    void removeListeners(MoveMarker* listener);

    /**
     * Add an event to the event queue
     *
     * @param positionToNotify the position for the event
     * @param event the event to add to the queue
     */
    void queueEvent(sf::Vector2i positionToNotify, Event* event);

    /**
     * Notify the listeners for each queued event
     */
    void notify();
};

#endif // CHESS_ACTION_LISTENER_TRACKER_H
