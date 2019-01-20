#include "targetingRule.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "piece.h"

// Public constructors

/**
 * Constructor
 */
TargetingRule::TargetingRule(const sf::Vector2i& offsetVector_, const std::string& targetName_) :
	offsetVector{offsetVector_},
	targetName{targetName_}
{
}

/**
 * Destructor
 */
TargetingRule::~TargetingRule() {
}



// Public methods

/**
 * Check whether a piece matches the targeting rule
 */
bool TargetingRule::matches(const Piece* candidate) const {
	// Check whether the piece name matches
	if (targetName != "*" && (candidate == nullptr || targetName != candidate->getDef()->name)) {
		return false;
	}

	return true;
}
