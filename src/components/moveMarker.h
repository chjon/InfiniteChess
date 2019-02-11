#ifndef CHESS_MOVE_MARKER_H
#define CHESS_MOVE_MARKER_H

#include <SFML/Graphics.hpp>
#include <tuple>
#include "../component_trackers/pieceTracker.h"

// Forward definitions
class Event;
class MoveDef;
class NumRule;
class Piece;
class TargetingRule;



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
	 * The next move marker
	 */
	MoveMarker* next;

	/**
	 * The previous move marker
	 */
	MoveMarker* prev;

	/**
	 * Whether the move marker meets a leaping rule
	 */
	bool meetsLeapingRule;

	/**
	 * Whether the move marker meets a scaling rule
	 */
	bool meetsScalingRule;

	/**
	 * Whether the piece meets an nth step rule
	 */
	bool meetsNthStepRule;

	/**
	 * The number of pieces in the way of this move marker
	 */
	unsigned int numObstructions;

	/**
	 * The move marker's targets
	 */
	std::map<sf::Vector2i, std::tuple<bool, Piece*, const TargetingRule*>, VectorUtils::cmpVectorLexicographically>* targets;

	// Helpers

	/**
	 * Determine whether the move marker meets a NumRule
	 */
	bool meetsNumRule(const std::vector<NumRule*>* numRules, unsigned int candidate, bool deleteList) const;

	/**
	 * Determine whether the position is being attacked
	 */
    bool isAttacked(PieceTracker* pieceTracker) const;

	// Event handlers

	/**
	 * Update the move marker chain's properties
	 */
	void update(Piece* piece, PieceTracker* pieceTracker, unsigned int numObstructions_);

	// Friends
	friend Piece;
	friend Renderer;

public:
	// Members
	/**
	 * The reflections that were done to produce this move marker
	 */
	const bool switchedX;
	const bool switchedY;
	const bool switchedXY;

	/**
	 * The scaling factor to get from the base move to this position
	 */
	const unsigned int lambda;

	// Constructors / Destructor
	MoveMarker(
		const Piece* rootPiece_, const MoveDef* rootMove_, sf::Vector2i baseVector, sf::Vector2i pos_,
		bool switchedX_, bool switchedY_, bool switchedXY_, unsigned int lambda_
	);
	~MoveMarker();

	// Event handlers

	/**
	 * Handle an event
	 *
	 * @param event the event to handle
	 */
	void handleEvent(Event* event);

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
	inline const sf::Vector2i getPos() const { return pos; }

	/**
	 * Iterate the move forward to get the position of the next move marker
	 */
	sf::Vector2i getNextPos() const;

	/**
	 * Get the next move marker
	 */
	const inline MoveMarker* getNext() const { return next; }

	/**
	 * Get the root piece for the move marker
	 */
	inline const Piece* getRootPiece() const { return rootPiece; }

	/**
	 * Get the root move for the move marker
	 */
	inline const MoveDef* getRootMove() const { return rootMove; }

	/**
	 * Get the base vector for the move marker
	 */
	inline const sf::Vector2i getBaseVector() const { return baseVector; }

	/**
	 * Get the number of pieces in the way of this move marker
	 */
	inline const unsigned int getNumObstructions() const { return numObstructions; }

	/**
	 * Get the targets for the move marker
	 */
	const std::vector<std::pair<Piece*, const TargetingRule*>>* getTargets() const;

	/**
	 * Determine whether the move marker is a valid move destination
	 */
	bool canMove(bool requireChainedMove) const;

	/**
	 * Get a list of all the potential target positions that the move marker is tracking
	 */
	const std::vector<sf::Vector2i>* getTargetedPositions() const;

	// Mutators

	/**
	 * Set the next move marker
	 */
	void setNext(MoveMarker* newNext);

	/**
	 * Set whether the move marker requires a leap
	 */
	void setNumObstructions(unsigned int numObstructions_);

	// Helpers

	/**
	 * Determine whether the move marker meets all of its targeting rules
	 */
	const bool meetsTargetingRules() const;
};

#endif // CHESS_MOVE_MARKER_H
