#ifndef CHESS_TARGETING_RULE_H
#define CHESS_TARGETING_RULE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Forward declarations
class Piece;



// Class declaration
class TargetingRule {
private:
public:
	// Members
    const sf::Vector2i offsetVector;
    const std::string targetName;

    // Constructors
    TargetingRule(const sf::Vector2i& offsetVector_, const std::string& targetName_);
    ~TargetingRule();

    // Methods
    bool matches (const Piece* candidate) const;
};

#endif // CHESS_TARGETING_RULE_H
