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

const sf::Color Piece::getTeam() const {
	return team;
}

const unsigned int Piece::getMoveCount() const {
	return moveCount;
}

const PieceDef* Piece::getDef() const {
	return pieceDef;
}

const MoveTracker* Piece::getMoveTracker() const {
	return moveTracker;
}

// Mutators

void Piece::setPos(sf::Vector2i newPos) {
    pos = newPos;
    onMove();
}



// Event handlers

void Piece::onStartUp() {
	moveTracker->onMove();
}

void Piece::onMove() {
	moveTracker->onMove();
	moveCount++;
}

void Piece::onCameraChange(PieceTracker* pieceTracker) {
	moveTracker->onCameraChange(pieceTracker);
}
