#ifndef CHESS_PIECE_MOVE_H
#define CHESS_PIECE_MOVE_H

#include <SFML/Graphics.hpp>

// Forward declarations

class GamePiece;



// Class definition

class PieceMove {
private:
	// Flags
    bool isAttack;
    bool allowScaling;
    bool canLeap;
    bool isXSymmetric;
    bool isYSymmetric;
    bool isXYSymmetric;

    // Members
	sf::Vector2i baseVector;

public:
	// Constructors
	PieceMove(sf::Vector2i baseVector_);
	~PieceMove();

	// Methods
	bool canMove (GamePiece* piece, const sf::Vector2i newPos);
};

#endif // CHESS_PIECE_MOVE_H
