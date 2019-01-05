#ifndef CHESS_MOVE_MARKER_H
#define CHESS_MOVE_MARKER_H

#include <SFML/Graphics.hpp>
#include "pieceTracker.h"

// Forward definitions
class GamePiece;
class PieceMove;
class PieceTracker;


/**
 * This class marks a possible movement position for optimizing movement checks
 */
class MoveMarker {
private:
	// Members

	/**
	 * The game piece that this move marker belongs to
	 */
	const GamePiece* rootPiece;

	/**
	 * The move that this move marker belongs to
	 */
	const PieceMove* rootMove;

	/**
	 * The base direction vector for this move marker
	 */
	const sf::Vector2i* baseVector;

	/**
	 * The previous move marker in the chain
	 */
	MoveMarker* prev;

	/**
	 * The next movement marker in the chain
	 */
	MoveMarker* next;

	/**
	 * The position of the movement marker
	 */
	const sf::Vector2i pos;

	/**
	 * The scaling factor to get from the base move to this position
	 */
	const unsigned int lambda;

public:
	// Constructors / Destructor
	MoveMarker(GamePiece* rootPiece_, PieceMove* rootMove_, sf::Vector2i* baseVector, sf::Vector2i pos_);
	~MoveMarker();

	// Accessors

	/**
	 * Get the move marker's position
	 */
	sf::Vector2i getPos() const;

	/**
	 * Iterate the move backward to get the position of the previous move marker
	 */
	sf::Vector2i getPrevPos() const;

	/**
	 * Iterate the move forward to get the position of the next move marker
	 */
	sf::Vector2i getNextPos() const;

	/**
	 * Get the previous move marker
	 */
	MoveMarker* getPrev() const;

	/**
	 * Get the next move marker
	 */
	MoveMarker* getNext() const;

	// Utility methods

	/**
	 * Determine whether the move marker is one of the ends of the move (beginning/end)
	 */
	bool isTerminal();

	/**
	 * Get rid of all the "next" move markers
	 */
	void destroyChildren();
};

#endif // CHESS_MOVE_MARKER_H
