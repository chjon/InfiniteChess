#ifndef CHESS_EVENT_H
#define CHESS_EVENT_H

#include <string>

// Forward declarations
class Piece;



// Class declaration
class Event {
public:
	// Constructors
	inline Event(Piece* piece_, const std::string& action_, const std::string& args_) :
		piece{piece_},
		action{action_},
		args{args_}
	{
	}

	inline ~Event(){}

	// Public API

	/**
	 * The piece for the action
	 */
	Piece* piece;

	/**
	 * The action that this object represents
	 */
	const std::string action;

	/**
	 * The arguments for the event
	 */
	const std::string args;
};

#endif // CHESS_EVENT_H
