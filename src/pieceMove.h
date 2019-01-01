#ifndef CHESS_PIECE_MOVE_H
#define CHESS_PIECE_MOVE_H

#include <SFML/Graphics.hpp>
#include "gamePiece.h"

// Forward declarations
class GamePiece;
class PieceTracker;



// Class declaration
class PieceMove {
private:
	enum MoveType {
		GENERAL, MOVE_ONLY, ATTACK_ONLY
	};

	// Flags
    bool allowScaling;
    bool canLeap;
    bool isXSymmetric;
    bool isYSymmetric;
    bool isXYSymmetric;

    // Members
    PieceTracker* pieceTracker;

    MoveType moveType;
	sf::Vector2i baseVector;

	// Utility methods
	bool canMove (GamePiece::Direction dir, const sf::Vector2i base, const sf::Vector2i newPos);

	// Friends
	friend GamePiece;
	friend PieceTracker;

public:
	// Constructors
	PieceMove(PieceTracker* p, sf::Vector2i baseVector_);
	~PieceMove();

	// Methods
	bool canMove (GamePiece* piece, const sf::Vector2i newPos);
};

#endif // CHESS_PIECE_MOVE_H
