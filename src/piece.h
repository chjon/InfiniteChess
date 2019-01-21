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
class MoveMarker;



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
	int lastMove;

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
	const sf::Color getTeam() const;
	const PieceDef::Direction getDir() const;
	const unsigned int getMoveCount() const;
	const PieceDef* getDef() const;
	const MoveTracker* getMoveTracker() const;
	inline const int getLastMove() const {
		return lastMove;
	}

	// Mutators
	void move(MoveMarker* dest);

	// Event handlers
	void onStartUp(PieceTracker* pieceTracker);
	void onCameraChange(PieceTracker* pieceTracker);
};

#endif // CHESS_PIECE_H
