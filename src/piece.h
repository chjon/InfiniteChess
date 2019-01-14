#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <SFML/Graphics.hpp>
#include "pieceDef.h"

// Forward declarations
class PieceDef;
class Renderer;
class MoveTracker;
class PieceMove;
class PieceTracker;



// Class declaration
class Piece {
private:
	// Piece definition
    const PieceDef* pieceDef;

    // Piece properties
    sf::Color team;
    sf::Vector2i pos;
	PieceDef::Direction dir;
	unsigned int moveCount;

	// Move tracker
	MoveTracker* moveTracker;

	// Friends
	friend Renderer;
	friend PieceMove;
	friend MoveDef;

public:
	// Constructors
	Piece(const PieceDef* pieceDef_, const sf::Color team_, sf::Vector2i pos_, PieceDef::Direction dir_);
	~Piece();

	// Accessors
	const sf::Vector2i getPos() const;
	const PieceDef* getDef() const;

	// Event handlers
	void onMove();
	void onCameraChange(PieceTracker* pieceTracker);
};

#endif // CHESS_PIECE_H
