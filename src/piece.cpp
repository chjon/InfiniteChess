#include "piece.h"
#include "moveTracker.h"

// Constructors

Piece::Piece(const PieceDef* pieceDef_, const sf::Color team_, sf::Vector2i pos_, PieceDef::Direction dir_) :
	pieceDef{pieceDef_},
	team{team_},
	pos{pos_},
	dir{dir_},
	moveCount{0},
	moveTracker{new MoveTracker(this)}
{
}

Piece::~Piece() {
}



// Accessors

const sf::Vector2i Piece::getPos() const {
	return pos;
}

const PieceDef* Piece::getDef() const {
	return pieceDef;
}

// Event handlers

void Piece::onMove() {
	moveTracker->onMove();
}

void Piece::onCameraChange(PieceTracker* pieceTracker) {
	moveTracker->onCameraChange(pieceTracker);
}
