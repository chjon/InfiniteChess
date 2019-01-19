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

	/**
	 * The next move marker
	 */
	MoveMarker* next;

	/**
	 * The previous move marker
	 */
	MoveMarker* prev;

	/**
	 * Whether a leap is required to get to the move marker
	 */
	bool requiresLeap;

	// Friends
	friend Piece;
	friend Renderer;

public:
	// Constructors / Destructor
	MoveMarker(const Piece* rootPiece_, const MoveDef* rootMove_, sf::Vector2i baseVector, sf::Vector2i pos_, unsigned int lambda_);
	MoveMarker(const Piece* rootPiece_, const MoveDef* rootMove_, sf::Vector2i baseVector, sf::Vector2i pos_);
	~MoveMarker();

	// Event handlers

	/**
	 * Update whether the move marker requires leaping when a piece leaves the tile
	 */
	void onPieceLeave(PieceTracker* pieceTracker);

	/**
	 * Update whether the move marker requires leaping when a piece enters the tile
	 */
	void onPieceEnter();

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
	 * Get the move marker's scale factor
	 */
	unsigned int getLambda() const;

	/**
	 * Get the next move marker
	 */
	MoveMarker* getNext() const;

	/**
	 * Get the root move for the move marker
	 */
	const MoveDef* getRootMove() const;

	/**
	 * Get the base vector for the move marker
	 */
	const sf::Vector2i getBaseVector() const;

	/**
	 * Get whether the move marker requires a leap
	 */
	const bool getRequiresLeap() const;

	/**
	 * Determine whether the move marker is a valid move destination
	 */
	bool canMove(PieceTracker* pieceTracker, const Piece* testPiece) const;
	bool canMove(PieceTracker* pieceTracker) const;

	// Mutators

	/**
	 * Set the next move marker
	 */
	void setNext(MoveMarker* newNext);

	/**
	 * Set whether the move marker requires a leap
	 */
	void setRequiresLeap(bool requiresLeap_);
};

#endif // CHESS_MOVE_MARKER_H
