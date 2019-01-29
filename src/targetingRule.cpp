#include "targetingRule.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "event.h"
#include "numRule.h"
#include "piece.h"
#include "resourceLoader.h"

// Initialize constants
const std::string TargetingRule::EMPTY_ONLY = "!";
const std::string TargetingRule::PIECE_ONLY = "+";
const std::string TargetingRule::ALL_PIECES = "*";

const std::map<std::string, TargetingRule::DataSpecifier> TargetingRule::DATA_SPECIFIERS = {
    {"LastMove", TargetingRule::DataSpecifier::LAST_MOVE},
    {"LastTurn", TargetingRule::DataSpecifier::LAST_TURN},
    {"NumMoves", TargetingRule::DataSpecifier::NUM_MOVES},
    {"SameTeam", TargetingRule::DataSpecifier::SAME_TEAM},
};

// Public constructors

/**
 * Constructor
 */
TargetingRule::TargetingRule(
	const sf::Vector2i& offsetVector_, const std::string& targetName_,
	const std::map<std::string, const NumRule*>* dataSpecifiers_, const std::vector<Event*>* actions_
) :
	dataSpecifiers{new std::map<DataSpecifier, const NumRule*>()},
	actions{actions_},
	offsetVector{offsetVector_},
	targetName{targetName_}
{
    for (
		std::map<std::string, const NumRule*>::const_iterator i = dataSpecifiers_->begin();
		i != dataSpecifiers_->end(); ++i
	) {
		std::map<std::string, DataSpecifier>::const_iterator found = DATA_SPECIFIERS.find(i->first);
		if (found == DATA_SPECIFIERS.end()) {
			throw new ResourceLoader::FileFormatException("Invalid data specifier: " + i->first);
		}

		dataSpecifiers->insert(std::make_pair(found->second, i->second));
    }
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
bool TargetingRule::matches(const Piece* rootPiece, const Piece* candidate) const {
	// Check whether the piece name matches
	if (candidate == nullptr) {
		if (targetName != EMPTY_ONLY && targetName != ALL_PIECES) {
			return false;
		}
	} else if (targetName != ALL_PIECES && targetName != PIECE_ONLY) {
		if (targetName != candidate->getDef()->name) {
			return false;
		}
	}

	// Check whether the target piece matches all the data specifiers
    for (
		std::map<DataSpecifier, const NumRule*>::const_iterator i = dataSpecifiers->begin();
		i != dataSpecifiers->end(); ++i
	) {
		switch (i->first) {
		case DataSpecifier::LAST_MOVE:
			if (!i->second->matches(candidate->getLastMove())) return false;
			break;
		case DataSpecifier::LAST_TURN:
			break;
		case DataSpecifier::NUM_MOVES:
            if (!i->second->matches(candidate->getMoveCount())) return false;
            break;
		case DataSpecifier::SAME_TEAM:
			if (!i->second->matches(rootPiece->getTeam() == candidate->getTeam())) return false;
			break;
        default:
        	break;
		}
    }

	return true;
}

const std::vector<Event*>* TargetingRule::getEvents() const {
    return actions;
}
