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
	inline const sf::Vector2i getPos() const { return pos; }
	inline const sf::Color getTeam() const { return team; }
	inline const PieceDef::Direction getDir() const { return dir; }
	inline const unsigned int getMoveCount() const { return moveCount; }
	inline const PieceDef* getDef() const { return pieceDef; }
	inline const MoveTracker* getMoveTracker() const { return moveTracker; }
	inline const int getLastMove() const { return lastMove; }
	inline const bool canMove(sf::Vector2i pos) const { return true; }

	// Mutators
	void move(MoveMarker* dest);

	// Event handlers
	void onStartUp(PieceTracker* pieceTracker);
	void onCameraChange(PieceTracker* pieceTracker);
};

#endif // CHESS_PIECE_H
