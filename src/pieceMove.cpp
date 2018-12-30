#include "pieceMove.h"

// Public constructors

PieceMove::PieceMove(sf::Vector2i baseVector_) :
	isAttack{true},
	allowScaling{false},
	canLeap{false},
    isXSymmetric{false},
    isYSymmetric{false},
    isXYSymmetric{false},
    baseVector{baseVector_}
{
}

PieceMove::~PieceMove() {
}



// Public methods

bool PieceMove::canMove(GamePiece* piece, const sf::Vector2i newPos) {
	return false;
}
