#ifndef CHESS_PIECE_DEF_LOADER_H
#define CHESS_PIECE_DEF_LOADER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>

// Forward declarations
class PieceDef;
class MoveDef;
class NumRule;


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


	// Helper methods
    static const bool isValidFileName(const std::string& fileName);
    static const PieceDef* pieceDefFromString(const std::string& pieceString);
    static const std::map<int, const MoveDef*>* getMovesFromString(const std::string& movesString);
    static const MoveDef* getMoveFromString(const std::string& moveString);
    static const unsigned int getNumArgs(const std::string& s);
    static const sf::Vector2i getVectorFromString(const std::string& s);
    static const std::vector<int>* getChainedMovesFromString(const std::string& s);
    static const std::vector<NumRule*>* getNumRulesFromString(const std::string& s);
	static const std::vector<std::string>* split(const std::string& s);
	static const std::string removeWhiteSpace(const std::string& fileName);
	static const bool isWhitespace(const char candidate);
	static std::map<std::string, const PieceDef*>* getPieceDefsFromStrings(const std::vector<std::string>* pieceStrings);

    // Validation methods
    static const void checkBracketEnclosed(const std::string& s);
    static const void checkNumArgs(const std::string& s, unsigned int expected);

public:
	// Methods
	static std::map<std::string, const PieceDef*>* loadPieceDefs (const std::string& fileName);
};

#endif // CHESS_PIECE_DEF_LOADER_H
