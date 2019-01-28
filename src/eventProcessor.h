#ifndef CHESS_EVENT_PROCESSOR
#define CHESS_EVENT_PROCESSOR

#include <vector>

// Forward declarations
class ActionListenerTracker;
class Event;
class PieceTracker;



// Class declaration
class EventProcessor {
private:
	// References
	PieceTracker* pieceTracker;
	ActionListenerTracker& actionListenerTracker;

	// Constants
	static const int NUM_QUEUES = 3;

	// Event queues
	const std::vector<std::vector<Event*>*> eventQueues = {
		new std::vector<Event*>(),
		new std::vector<Event*>(),
		new std::vector<Event*>(),
	};

	// Helpers

	/**
	 * Execute an event
	 */
	void execute(Event* event);

public:
	// Constants
	static const int START = 0;
	static const int EVENT = 1;
	static const int AFTER = 2;

	// Constructors
    EventProcessor(PieceTracker* pieceTracker_, ActionListenerTracker& actionListenerTracker_);
    ~EventProcessor();

	// Public API

	/**
	 * Add an event to an event queue
	 *
	 * @param queueIndex the index of the queue to add to
	 * @param event the event to add to the queue
	 */
	void insertInQueue(int queueIndex, Event* event);

	/**
	 * Execute all of the events
	 */
    void executeEvents();
};

#endif // CHESS_EVENT_PROCESSOR
