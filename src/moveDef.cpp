#include "moveDef.h"

// Public constructors

/**
 * Constructor
 */
MoveDef::MoveDef(
	int index_, sf::Vector2i baseVector_, bool attacksFriendlies_, bool attacksEnemies_,
	bool movesEmpty_, bool canLeap_, bool endsTurn_, bool isXSymmetric_, bool isYSymmetric_, bool isXYSymmetric_
) :
	index{index_},
	baseVector{baseVector_},
	attacksFriendlies{attacksFriendlies_},
	attacksEnemies{attacksEnemies_},
    movesEmpty{movesEmpty_},
    canLeap{canLeap_},
    endsTurn{endsTurn_},
    isXSymmetric{isXSymmetric_},
    isYSymmetric{isYSymmetric_},
    isXYSymmetric{isXYSymmetric_}
{
}

/**
 * Destructor
 */
MoveDef::~MoveDef() {
}
