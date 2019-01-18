#ifndef CHESS_MOVE_DEF_H
#define CHESS_MOVE_DEF_H

#include <SFML/Graphics.hpp>

// Forward declarations
class MoveMarker;
class NumRule;
class Piece;



// Class declaration
class MoveDef {
private:
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
	const std::vector<int>* targettingRules;

	// Constructors
	MoveDef(
		int index_, sf::Vector2i baseVector_, bool attacksFriendlies_, bool attacksEnemies_,
		bool movesEmpty_, bool canLeap, bool endsTurn, bool isXSymmetric, bool isYSymmetric, bool isXYSymmetric,
		const std::vector<int>* chainedMoves, const std::vector<NumRule*>* scalingRules,
		const std::vector<NumRule*>* nthStepRules
	);

	~MoveDef();

	// Methods
	const std::vector<MoveMarker*>* generateMarkers(const Piece* piece) const;
};

#endif // CHESS_MOVE_DEF_H
