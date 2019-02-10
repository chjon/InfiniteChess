#include "pieceTracker.h"

#include <map>
#include "../game.h"
#include "../components/moveMarker.h"
#include "moveTracker.h"
#include "../components/piece.h"



// Public constructors

/**
 * Constructor
 */
PieceTracker::PieceTracker(Game* g) :
	game{g},
	pieceDefs{nullptr},
	pieces{nullptr}
{
}

/**
 * Destructor
 */
PieceTracker::~PieceTracker() {
	// Delete all the stored pieces
	if (pieces != nullptr) {
		for (std::map<sf::Vector2i, Piece*>::iterator it = pieces->begin(); it != pieces->end(); it++) {
			delete it->second;
		}

		// Clean up
		delete pieces;
		pieces = nullptr;
	}

	// Delete all the piece definitions
	if (pieceDefs != nullptr) {
		for (std::map<std::string, const PieceDef*>::iterator it = pieceDefs->begin(); it != pieceDefs->end(); it++) {
			delete it->second;
		}

		// Clean up
		delete pieceDefs;
		pieceDefs = nullptr;
	}
}

// Clear pieces
void PieceTracker::clearPieces() {
	if (pieces != nullptr) {
		for (std::map<sf::Vector2i, Piece*>::iterator it = pieces->begin(); it != pieces->end(); it++) {
			delete it->second;
		}

		// Clean up
		delete pieces;
		pieces = nullptr;
	}
}

// Public event handlers

/**
 * Handle the initial loading of pieces
 */
void PieceTracker::onStartup(
	std::map<std::string, const PieceDef*>* defs,
	std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>* startPieces
) {
	pieceDefs = defs;
	pieces = startPieces;
}

/**
 * Update the pieces when the camera changes
 */
void PieceTracker::onCameraChange() {
	for (std::map<sf::Vector2i, Piece*>::iterator i = pieces->begin(); i != pieces->end(); ++i) {
		Piece* piece = i->second;
		piece->onCameraChange(this);
	}
}

/**
 * Notify the game that a new move marker was generated
 */
void PieceTracker::onGeneration(MoveMarker* generated) {
    game->onGeneration(generated);
}



// Accessors

/**
 * Determine whether a certain position is within the bounds of the screen
 */
bool PieceTracker::isRenderable(sf::Vector2i pos) const {
	return game->renderer->isRenderable(pos);
}



// Public methods

/**
 * Add a piece to the piece tracker
 */
void PieceTracker::addPiece(Piece* piece) {
	// Check whether a piece is already at the desired location
	if (pieces->find(piece->getPos()) != pieces->end()) return;

	// Insert the piece
	pieces->insert(std::make_pair(piece->getPos(), piece));
}

/**
 * Remove a piece from the piece tracker
 */
bool PieceTracker::removePiece(sf::Vector2i pos) {
	Piece* piece = getPiece(pos);

	if (piece == nullptr) {
		return false;
	}

	pieces->erase(pieces->find(pos));
	return true;
}

/**
 * Get the piece at a certain spot
 */
Piece* PieceTracker::getPiece(sf::Vector2i pos) const {
	const std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>::const_iterator it = pieces->find(pos);

	// Return the null pointer if the piece is not in the map
	if (it == pieces->end()) {
		return nullptr;
	}

	return it->second;
}

/**
 * Get all the pieces
 */
const std::vector<Piece*>* PieceTracker::getPieces() const {
	std::vector<Piece*>* pieceList = new std::vector<Piece*>();
	for (std::map<sf::Vector2i, Piece*>::const_iterator i = pieces->begin(); i != pieces->end(); ++i) {
        pieceList->push_back(i->second);
	}

	return pieceList;
}

/**
 * Get the first valid move marker for the position
 */
MoveMarker* PieceTracker::getValidMove(Piece* piece, sf::Vector2i dest) {
	const std::vector<MoveMarker*>* markers = piece->getMoveTracker()->getMoveMarkers(dest);
	MoveMarker* validMove = nullptr;

	for (std::vector<MoveMarker*>::const_iterator i = markers->begin(); i != markers->end(); ++i) {
        if ((*i)->canMove()) {
			validMove = *i;
			break;
        }
	}

	// Clean up
	delete markers;
	markers = nullptr;

    return validMove;
}

/**
 * Get all the move markers at a position
 */
std::vector<MoveMarker*>* PieceTracker::getMoveMarkers(sf::Vector2i pos) const {
	std::vector<MoveMarker*>* markers = new std::vector<MoveMarker*>();

	// Iterate through each piece
	for (std::map<
			sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically
		>::const_iterator i = pieces->begin(); i != pieces->end(); ++i
	) {
        const std::vector<MoveMarker*>* markersForPiece = i->second->getMoveTracker()->getMoveMarkers(pos);

		// Add all the move markers
		for (std::vector<MoveMarker*>::const_iterator j = markersForPiece->begin(); j != markersForPiece->end(); ++j) {
            markers->push_back(*j);
		}

		// Clean up
		delete markersForPiece;
		markersForPiece = nullptr;
	}

	return markers;
}

/**
 * Get a string of all the pieces
 */
std::string PieceTracker::piecesToString() const {
	std::string output = "[\n";
	for (std::map<sf::Vector2i, Piece*>::const_iterator i = pieces->begin(); i != pieces->end(); ++i) {
		output += i->second->toString() + ",\n";
	}

	return output + "]";
}
