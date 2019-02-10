#ifndef CHESS_MOVE_DEF_H
#define CHESS_MOVE_DEF_H

#include <SFML/Graphics.hpp>
#include "pieceDef.h"

// Forward declarations
class MoveMarker;
class NumRule;
class Piece;
class TargetingRule;



// Class declaration
class MoveDef {
public:
	// Members
	const int index;

	const sf::Vector2i baseVector;

	const bool canLeap;
	const bool endsTurn;
	const bool isXSymmetric;
	const bool isYSymmetric;
	const bool isXYSymmetric;

	const std::vector<int>* chainedMoves;
	const std::vector<NumRule*>* scalingRules;
	const std::vector<NumRule*>* nthStepRules;
	const std::vector<const TargetingRule*>* targetingRules;

	unsigned int constantMultiple;

	// Constructors
	MoveDef(
		int index_, sf::Vector2i baseVector_, bool canLeap, bool endsTurn,
		bool isXSymmetric, bool isYSymmetric, bool isXYSymmetric,
		const std::vector<int>* chainedMoves, const std::vector<NumRule*>* scalingRules,
		const std::vector<NumRule*>* nthStepRules, const std::vector<const TargetingRule*>* targetingRules_
	);

	~MoveDef();

	// Helper methods
	static sf::Vector2i rotate(const sf::Vector2i original, const PieceDef::Direction dir);

	// Methods
	const std::vector<MoveMarker*>* generateMarkers(const Piece* piece) const;

	bool meetsNthStepRules(const unsigned int moveCount) const;
};

#endif // CHESS_MOVE_DEF_H
