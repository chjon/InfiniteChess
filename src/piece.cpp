#include "piece.h"

// Constructors

Piece::Piece(const PieceDef* pieceDef_, const sf::Color team_, sf::Vector2i pos_, PieceDef::Direction dir_) :
	pieceDef{pieceDef_},
	team{team_},
	pos{pos_},
	dir{dir_},
	moveCount{0}
{
}

Piece::~Piece() {
}



// Accessors

const sf::Vector2i Piece::getPos() const {
	return pos;
}
