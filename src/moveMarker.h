#ifndef CHESS_MOVE_MARKER_H
#define CHESS_MOVE_MARKER_H

#include <SFML/Graphics.hpp>
#include "pieceTracker.h"

// Forward definitions
class Piece;
class MoveDef;



/**
 * This class marks a possible movement position for optimizing movement checks
 */
class MoveMarker {
private:
	// Members

	/**
	 * The game piece that this move marker belongs to
	 */
	const Piece* rootPiece;

	/**
	 * The move that this move marker belongs to
	 */
	const MoveDef* rootMove;

	/**
	 * The base direction vector for this move marker
	 */
	const sf::Vector2i baseVector;

	/**
	 * The position of the movement marker
	 */
	const sf::Vector2i pos;

	/**
	 * The scaling factor to get from the base move to this position
	 */
	const unsigned int lambda;

	// Friends
	friend Piece;
	friend Renderer;

public:
	// Constructors / Destructor
	MoveMarker(const Piece* rootPiece_, const MoveDef* rootMove_, sf::Vector2i baseVector, sf::Vector2i pos_);
	~MoveMarker();

	// Accessors

	/**
	 * Get the move marker's position
	 */
	sf::Vector2i getPos() const;

	/**
	 * Iterate the move forward to get the position of the next move marker
	 */
	sf::Vector2i getNextPos() const;

	/**
	 * Get the root move for the move marker
	 */
	const MoveDef* getRootMove() const;

	/**
	 * Get the base vector for the move marker
	 */
	const sf::Vector2i getBaseVector() const;

	/**
	 * Determine whether the move marker is a valid move destination
	 */
	bool canMove() const;
};

#endif // CHESS_MOVE_MARKER_H
