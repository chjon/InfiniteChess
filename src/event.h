#ifndef CHESS_EVENT_H
#define CHESS_EVENT_H

#include <string>

// Forward declarations
class Piece;



// Class declaration
class Event {
public:
	// Constructors
	inline Event(const Piece* piece_, const std::string& action_) :
		piece{piece_},
		action{action_}
	{
	}

	~Event();

	// Public API

	/**
	 * The piece that triggered the action
	 */
	const Piece* piece;

	/**
	 * The action that this object represents
	 */
	const std::string action;
};

#endif // CHESS_EVENT_H
