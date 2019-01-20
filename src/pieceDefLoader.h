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
    static const std::string EXTENSION;
	static const std::vector<char>* WHITESPACE;

    static const char BRACKET_OPEN   = '[';
	static const char BRACKET_CLOSE  = ']';
	static const char SEPARATOR      = ',';
	static const char COMMENT_MARKER = '#';

	static const int NUM_MOVE_ARGS = 7;
	static const int NUM_TARGETTING_RULE_ARGS = 4;

	// Object generation methods
    static const PieceDef* getPieceDefFromString(const std::string& pieceString);
    static const MoveDef* getMoveFromString(const std::string& moveString);
    static const sf::Vector2i getVectorFromString(const std::string& s);
    static const TargetingRule* getTargetingRuleFromString(const std::string& s);
    template <typename T> static std::vector<T>* getListFromString(
		const std::string& listString,
		T(*objectFromString)(const std::string& s)
	);
	template <typename K, typename V> static std::map<K, V>* getMapFromString(
		const std::string& listString,
		K(*keyFromObject)(V),
		V(*objectFromString)(const std::string&)
	);

	// Helper methods
	static const unsigned int getNumArgs(const std::string& s);
	static const std::vector<std::string>* split(const std::string& s);
	static const std::string removeWhiteSpace(const std::string& fileName);
	static const bool isWhitespace(const char candidate);

    // Validation methods
    static const bool isValidFileName(const std::string& fileName);
    static const void checkBracketEnclosed(const std::string& s);
    static const void checkNumArgs(const std::string& s, unsigned int expected);

public:
	// Methods
	static std::map<std::string, const PieceDef*>* loadPieceDefs (const std::string& fileName);
};

#endif // CHESS_PIECE_DEF_LOADER_H
