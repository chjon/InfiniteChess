#ifndef CHESS_TARGETING_RULE_H
#define CHESS_TARGETING_RULE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Forward declarations
class Event;
class NumRule;
class Piece;



// Class declaration
class TargetingRule {
private:
	// Constants
	static const std::string EMPTY_ONLY;
	static const std::string PIECE_ONLY;
	static const std::string ALL_PIECES;

	enum DataSpecifier {
		LAST_MOVE, LAST_TURN, NUM_MOVES, SAME_TEAM
	};

	static const std::map<std::string, DataSpecifier> DATA_SPECIFIERS;

	// Members
	std::map<DataSpecifier, const NumRule*>* dataSpecifiers;
	const std::vector<Event*>* actions;

public:
	// Members
    const sf::Vector2i offsetVector;
    const std::string targetName;

    // Constructors
    TargetingRule(
		const sf::Vector2i& offsetVector_, const std::string& targetName_,
		const std::map<std::string, const NumRule*>* dataSpecifiers_, const std::vector<Event*>* actions_
	);
    ~TargetingRule();

    // Methods
    bool matches (const Piece* rootPiece, const Piece* candidate) const;
    const std::vector<Event*>* getEvents() const;
};

#endif // CHESS_TARGETING_RULE_H
