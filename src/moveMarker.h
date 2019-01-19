#ifndef CHESS_MOVE_MARKER_H
#define CHESS_MOVE_MARKER_H

#include <SFML/Graphics.hpp>
#include "pieceTracker.h"

// Forward definitions
class MoveDef;
class NumRule;
class Piece;



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

	/**
	 * Whether the move marker meets the attack requirements
	 */
	bool meetsAttackRequirements;

	/**
	 * Whether the move marker meets a scaling rule
	 */
	bool meetsScalingRule;

	/**
	 * Whether the piece meets an nth step rule
	 */
	bool meetsNthStepRule;

	// Helpers

	/**
	 * Determine whether a move meets the attack requirements
	 */
	void updateMeetsAttackRequirements(PieceTracker* pieceTracker);

	/**
	 * Determine whether the move marker meets a NumRule
	 */
	bool meetsNumRule(const std::vector<NumRule*>* numRules, unsigned int candidate, bool deleteList) const;

	/**
	 * Update the move marker chain when a piece leaves the tile
	 */
	void onPieceLeaveNext(Piece* piece, PieceTracker* pieceTracker);

	/**
	 * Update the move marker chain when a piece enters the tile
	 */
	void onPieceEnterNext(Piece* piece, PieceTracker* pieceTracker);

	/**
	 * Determine whether the position is being attacked
	 */
    bool isAttacked(PieceTracker* pieceTracker) const;

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
	 * Update the move marker when it is generated
	 */
	void onGeneration(PieceTracker* pieceTracker);

	/**
	 * Update the move marker when a piece leaves the tile
	 */
	void onPieceLeave(Piece* piece, PieceTracker* pieceTracker);

	/**
	 * Update the move marker when a piece enters the tile
	 */
	void onPieceEnter(Piece* piece, PieceTracker* pieceTracker);

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
