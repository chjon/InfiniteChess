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
	terminalMoveMarkers.clear();
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
GamePiece::GamePiece(PieceTracker* pieceTracker_, const GamePiece* piece, const sf::Color team_, sf::Vector2i pos_, Direction dir_) :
	pieceTracker{pieceTracker_},
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
	deleteMoveMarkers();
	moveSet = nullptr;
}



// Public methods

/**
 * Update the piece's move markers when the piece moves
 */
void GamePiece::onMove() {
	// Delete all current move markers
	deleteMoveMarkers();

	// Generate new move markers
	for (std::vector<PieceMove*>::const_iterator i = moveSet->begin(); i != moveSet->end(); ++i) {
		(*i)->generateMoveMarkers(this);
	}

	onCameraChange();
}

/**
 * Update the piece's move markers when the camera changes
 */
void GamePiece::onCameraChange() {
	std::vector<MoveMarker*> temp;

	// Update the terminal move markers
	for (std::vector<MoveMarker*>::iterator it = terminalMoveMarkers.begin(); it != terminalMoveMarkers.end(); ++it) {
		MoveMarker* terminalMarker = *it;
		const PieceMove* rootMove = terminalMarker->rootMove;

		sf::Vector2i nextPos = terminalMarker->getNextPos();
		MoveMarker* prev = terminalMarker;

		// Generate all renderable move markers
		while (pieceTracker->isRenderable(nextPos)) {
			terminalMarker = new MoveMarker(this, rootMove, terminalMarker->baseVector, nextPos);
			terminalMarker->setPrev(prev);
			prev = terminalMarker;

			moveMarkers.insert(std::make_pair(nextPos, terminalMarker));
			nextPos = terminalMarker->getNextPos();
		}

		// Store the new terminal move marker
		temp.push_back(terminalMarker);
	}

	terminalMoveMarkers.clear();

	// Push the terminal move markers back into the member vector
	for (std::vector<MoveMarker*>::iterator i = temp.begin(); i != temp.end(); ++i) {
		terminalMoveMarkers.push_back(*i);
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
	onMove();
}
