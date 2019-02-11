#include "piece.h"

#include "moveDef.h"
#include "moveMarker.h"
#include "../component_trackers/moveTracker.h"

// Constructors

Piece::Piece(
	const PieceDef* pieceDef_, const unsigned int team_, sf::Vector2i pos_,
	PieceDef::Direction dir_, const unsigned int moveCount_, const int lastMove_
) :
	pieceDef{pieceDef_},
	team{team_},
	pos{pos_},
	dir{dir_},
	moveCount{moveCount_},
	lastMove{lastMove_},
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

const MoveMarker* Piece::getValidMove(sf::Vector2i pos, bool requireChainedMove) const {
	MoveMarker* validMove = nullptr;

	const std::vector<MoveMarker*>* markers = moveTracker->getMoveMarkers(pos);
	for (std::vector<MoveMarker*>::const_iterator i = markers->begin(); i != markers->end(); ++i) {
        if ((*i)->canMove(requireChainedMove)) {
			validMove = *i;
			break;
        }
	}

	// Clean up
	delete markers;
	markers = nullptr;

	return validMove;
}

bool Piece::isChainedMove(int moveIndex) const {
	const std::map<int, const MoveDef*>::const_iterator lastMoveIter = pieceDef->moves->find(lastMove);
	if (lastMoveIter == pieceDef->moves->end()) {
		return true;
	}

	const std::vector<int>* chainedMoves = lastMoveIter->second->chainedMoves;
	for (std::vector<int>::const_iterator i = chainedMoves->begin(); i != chainedMoves->end(); ++i) {
        if (moveIndex == *i) {
			return true;
        }
	}

	return false;
}


// Mutators

void Piece::setPos(sf::Vector2i dest) {
	pos = dest;
	moveCount++;
}

void Piece::move(sf::Vector2i dest) {
    pos = dest;
    moveCount++;
    moveTracker->onMove();
}



// Event handlers

void Piece::onStartup(PieceTracker* pieceTracker) {
    moveTracker->onStartup(pieceTracker);
}

void Piece::onCameraChange(PieceTracker* pieceTracker) {
	moveTracker->onCameraChange(pieceTracker);
}

void Piece::onMove() {
	moveTracker->onMove();
}
