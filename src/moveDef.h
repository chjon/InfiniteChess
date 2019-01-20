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

	const bool attacksFriendlies;
	const bool attacksEnemies;
	const bool movesEmpty;
	const bool canLeap;
	const bool endsTurn;
	const bool isXSymmetric;
	const bool isYSymmetric;
	const bool isXYSymmetric;

	const std::vector<int>* chainedMoves;
	const std::vector<NumRule*>* scalingRules;
	const std::vector<NumRule*>* nthStepRules;
	const std::vector<const TargetingRule*>* targetingRules;

	// Constructors
	MoveDef(
		int index_, sf::Vector2i baseVector_, bool attacksFriendlies_, bool attacksEnemies_,
		bool movesEmpty_, bool canLeap, bool endsTurn, bool isXSymmetric, bool isYSymmetric, bool isXYSymmetric,
		const std::vector<int>* chainedMoves, const std::vector<NumRule*>* scalingRules,
		const std::vector<NumRule*>* nthStepRules, const std::vector<const TargetingRule*>* targetingRules_
	);

	~MoveDef();

	// Helper methods
	static sf::Vector2i rotate(const sf::Vector2i original, const PieceDef::Direction dir);
	static sf::Vector2i reflect(const sf::Vector2i original, bool reflectX, bool reflectY, bool reflectXY);

	// Methods
	const std::vector<MoveMarker*>* generateMarkers(const Piece* piece) const;
};

#endif // CHESS_MOVE_DEF_H
