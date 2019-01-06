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

	// Friends
	friend GamePiece;
	friend PieceTracker;
	friend MoveMarker;
	friend ResourceLoader;

public:
	// Constructors
	PieceMove(PieceTracker* p, sf::Vector2i baseVector_);
	~PieceMove();

	// Methods
	void generateMoveMarkers(GamePiece* piece);
};

#endif // CHESS_PIECE_MOVE_H
