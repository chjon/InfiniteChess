#include "gamePiece.h"
#include "pieceMove.h"
#include "moveMarker.h"

// Private utility methods

/**
 * Delete the piece's current move markers
 */
void GamePiece::deleteMoveMarkers() {
    for (std::map<
			sf::Vector2i,
			MoveMarker*,
			VectorUtils::cmpVectorLexicographically
		>::iterator i = moveMarkers.begin(); i != moveMarkers.end(); ++i
	) {
		delete i->second;
    }

    moveMarkers.clear();
}

/**
 * Do a full delete for the definition
 */
void GamePiece::definitionDelete() {
    // Delete each of the moves in the move set
    for (std::vector<PieceMove*>::const_iterator it = moveSet->begin(); it != moveSet->end(); it++) {
        delete *it;
    }

    delete moveSet;
    moveSet = nullptr;
}


// Public constructors

/**
 * Constructor for creating template pieces
 */
GamePiece::GamePiece(const std::string n, const std::vector<PieceMove*>* m) :
	name{n},
	moveSet{m}
{
}

/**
 * "Copy" constructor for copying from the definition
 */
GamePiece::GamePiece(const GamePiece* piece, const sf::Color team_, sf::Vector2i pos_, Direction dir_) :
	name{piece->name},
	moveSet{piece->moveSet},
	team{team_},
	pos{pos_},
	dir{dir_}
{
}

/**
 * Destructor
 */
GamePiece::~GamePiece() {
	moveSet = nullptr;
}



// Public methods

/**
 * Update the piece's move markers
 */
void GamePiece::updateMoveMarkers() {
	// Delete all current move markers
	deleteMoveMarkers();

    // Generate new move markers
    for (std::vector<PieceMove*>::const_iterator i = moveSet->begin(); i != moveSet->end(); ++i) {
        (*i)->generateMoveMarkers(this);
    }
}

/**
 * Check whether the piece can move to the requested spot
 *
 * @param newPos the desired position
 */
bool GamePiece::canMove(const sf::Vector2i newPos) {
	std::map<sf::Vector2i, MoveMarker*>::iterator it = moveMarkers.find(newPos);
	if (it == moveMarkers.end()) {
		return false;
	}

	return it->second->canMove();
}

/**
 * Move the piece
 */
void GamePiece::move(const sf::Vector2i newPos) {
	pos = newPos;
	updateMoveMarkers();
}
