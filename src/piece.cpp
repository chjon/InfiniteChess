#include "piece.h"

#include "moveDef.h"
#include "moveMarker.h"
#include "moveTracker.h"

// Constructors

Piece::Piece(const PieceDef* pieceDef_, const sf::Color team_, sf::Vector2i pos_, PieceDef::Direction dir_) :
	pieceDef{pieceDef_},
	team{team_},
	pos{pos_},
	dir{dir_},
	moveCount{0},
	lastMove{-1},
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

const PieceDef::Direction Piece::getDir() const {
	return dir;
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

void Piece::move(MoveMarker* dest) {
    pos = dest->getPos();
    lastMove = dest->getRootMove()->index;
    moveCount++;
    moveTracker->onMove();
}



// Event handlers

void Piece::onStartUp(PieceTracker* pieceTracker) {
	moveTracker->onStartUp(pieceTracker);
}

void Piece::onCameraChange(PieceTracker* pieceTracker) {
	moveTracker->onCameraChange(pieceTracker);
}
