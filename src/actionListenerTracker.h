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
    std::map<sf::Vector2i, std::map<std::string, MoveMarker*>*, VectorUtils::cmpVectorLexicographically> actionListeners;

    // Helpers
    /**
	 * Add a move listener
	 *
	 * @param positionToNotify the position to listen to
	 * @param listener the move marker to notify upon update of the trigger position
	 */
    void addListener(sf::Vector2i positionToNotify, MoveMarker* listener);

    /**
     * Generate a key from a move marker
     */
    static std::string generateKey(MoveMarker* marker);

public:
    // Constructors
    ActionListenerTracker();
	~ActionListenerTracker();

	// Public API

    /**
	 * Add a move listener
	 *
	 * @param marker the move marker to notify upon update of the trigger position
	 */
	void addListeners(MoveMarker* marker);

    /**
	* Remove all the action listeners for a given move marker
	*
	* @param listener the move marker to remove move listeners for
	*/
	void removeListeners(MoveMarker* listener);

    /**
     * Notify the action listeners that are listening to the given position
     *
     * @param positionToNotify the position to notify
     * @param event the action that triggered the notification
     */
    void notify(sf::Vector2i positionToNotify, Event* event);
};

#endif // CHESS_ACTION_LISTENER_TRACKER_H
