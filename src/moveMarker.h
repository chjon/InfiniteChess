#ifndef CHESS_MOVE_MARKER_H
#define CHESS_MOVE_MARKER_H

#include <SFML/Graphics.hpp>
#include "pieceTracker.h"

// Forward definitions
class GamePiece;
class PieceMove;



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
	friend GamePiece;
	friend Renderer;

public:
	// Constructors / Destructor
	MoveMarker(GamePiece* rootPiece_, const PieceMove* rootMove_, sf::Vector2i baseVector, sf::Vector2i pos_);
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
	 * Determine whether the move marker is a valid move destination
	 */
	bool canMove() const;
};

#endif // CHESS_MOVE_MARKER_H
