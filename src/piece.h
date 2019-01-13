#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <SFML/Graphics.hpp>
#include "pieceDef.h"

// Forward declarations
class PieceDef;
class Renderer;
class PieceMove;



// Class declaration
class Piece {
private:
	// Piece definition
    const PieceDef* pieceDef;

    // Members
    sf::Color team;
    sf::Vector2i pos;
	PieceDef::Direction dir;
	unsigned int moveCount;

	// Friends
	friend Renderer;
	friend PieceMove;

public:
	// Constructors
	Piece(const PieceDef* pieceDef_, const sf::Color team_, sf::Vector2i pos_, PieceDef::Direction dir_);
	~Piece();

	// Accessors
	const sf::Vector2i getPos() const;
};

#endif // CHESS_PIECE_H
