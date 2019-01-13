#ifndef CHESS_MOVE_DEF_H
#define CHESS_MOVE_DEF_H

#include <SFML/Graphics.hpp>

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

	const std::vector<int> chainedMoves;
	const std::vector<int> scalingRules;
	const std::vector<int> nthStepRules;
	const std::vector<int> targettingRules;

	// Constructors
	MoveDef(
		int index_, sf::Vector2i baseVector_, bool attacksFriendlies_, bool attacksEnemies_,
		bool movesEmpty_, bool canLeap, bool endsTurn, bool isXSymmetric, bool isYSymmetric, bool isXYSymmetric
	);

	~MoveDef();
};

#endif // CHESS_MOVE_DEF_H
