#ifndef CHESS_PIECE_DEF_LOADER_H
#define CHESS_PIECE_DEF_LOADER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

// Forward declarations
class PieceDef;
class MoveDef;
class NumRule;
class TargetingRule;


// Class declaration
class PieceDefLoader {
private:
    // Constants
	static const int NUM_MOVE_ARGS = 7;
	static const int NUM_TARGETTING_RULE_ARGS = 4;

	// Object generation methods
    static const PieceDef* getPieceDefFromString(const std::string& pieceString);
    static const MoveDef* getMoveFromString(const std::string& moveString);
    static const TargetingRule* getTargetingRuleFromString(const std::string& s);

	// Helper methods
	static const std::string removeWhiteSpace(const std::string& fileName);
	static const bool isWhitespace(const char candidate);

public:
	// Methods
	static std::map<std::string, const PieceDef*>* loadPieceDefs (
		const std::string& fileName, const std::string& extension
	);
};

#endif // CHESS_PIECE_DEF_LOADER_H
