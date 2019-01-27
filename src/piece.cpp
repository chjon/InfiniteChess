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

/**
 * Get all the targets for the move markers at a given position
 *
 * @param pos the position of the move markers
 */
const std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>* Piece::getTargets(sf::Vector2i pos) const {
	return moveTracker->getTargets(pos);
}

const bool Piece::canMove(sf::Vector2i pos) const {
	bool canMove = false;

	const std::vector<MoveMarker*>* markers = moveTracker->getMoveMarkers(pos);
	for (std::vector<MoveMarker*>::const_iterator i = markers->begin(); i != markers->end(); ++i) {
        canMove |= (*i)->canMove();
	}

	// Clean up
	delete markers;
	markers = nullptr;

	return canMove;
}


// Mutators

void Piece::setPos(sf::Vector2i dest) {
	pos = dest;
	moveCount++;
}

void Piece::move(sf::Vector2i dest) {
    pos = dest;
    //lastMove = dest->getRootMove()->index;
    moveCount++;
    moveTracker->onMove();
}



// Event handlers

void Piece::onCameraChange(PieceTracker* pieceTracker) {
	moveTracker->onCameraChange(pieceTracker);
}

void Piece::onMove() {
	moveTracker->onMove();
}
