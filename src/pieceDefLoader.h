#ifndef CHESS_PIECE_DEF_LOADER_H
#define CHESS_PIECE_DEF_LOADER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>

// Forward declarations
class PieceDef;
class MoveDef;



// Class declaration
class PieceDefLoader {
private:
    // Constants
    static const std::string EXTENSION;

    static const char BRACKET_OPEN  = '[';
    static const char BRACKET_CLOSE = ']';
    static const char SEPARATOR     = ',';

    static const int NUM_MOVE_ARGS = 7;

	// Methods
    static const bool isValidFileName(const std::string& fileName);
    static const PieceDef* pieceDefFromString(const std::string& pieceString);
    static const std::map<int, const MoveDef*>* getMovesFromString(const std::string& movesString);
    static const MoveDef* getMoveFromString(const std::string& moveString);
    static const int getNumArgs(const std::string& s);
    static const sf::Vector2i getVectorFromString(const std::string& s);

public:
	// Methods
	static std::map<std::string, const PieceDef*>* loadPieceDefs (const std::string& fileName);
};

#endif // CHESS_PIECE_DEF_LOADER_H
