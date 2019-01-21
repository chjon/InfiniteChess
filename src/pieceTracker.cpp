#include "pieceTracker.h"

#include <map>
#include "game.h"
#include "moveMarker.h"
#include "moveTracker.h"
#include "piece.h"
#include <iostream>

// Private utility methods

/**
 * Create the move markers for each piece
 */
void PieceTracker::generateMoveMarkers() {
	// Generate and add the move markers for each piece
	for (std::map<sf::Vector2i, Piece*>::iterator it = pieces.begin(); it != pieces.end(); ++it) {
		it->second->onStartUp(this);
	}
}



// Public constructors

/**
 * Constructor
 */
PieceTracker::PieceTracker(Game* g) :
	game{g}
{
}

/**
 * Destructor
 */
PieceTracker::~PieceTracker() {
	// Delete all the stored pieces
	for (std::map<sf::Vector2i, Piece*>::iterator it = pieces.begin(); it != pieces.end(); it++) {
		delete it->second;
	}

	pieceDefs = nullptr;
}

// Public event handlers

/**
 * Handle the initial loading of pieces
 */
void PieceTracker::onStartup() {
	sf::Color TEAM_1(255, 255, 255, 255);
	sf::Color TEAM_2( 32,  32,  32, 255);

	// Create pawns
	for (int i = -4; i < 4; i++) {
		addPiece("Pawn", TEAM_1, sf::Vector2i(i, -3), PieceDef::Direction::DOWN);
		addPiece("Pawn", TEAM_2, sf::Vector2i(i, 2), PieceDef::Direction::UP);
	}

	// Create rooks
	addPiece("Rook", TEAM_1, sf::Vector2i(-4, -4), PieceDef::Direction::DOWN);
	addPiece("Rook", TEAM_1, sf::Vector2i(3, -4), PieceDef::Direction::DOWN);
	addPiece("Rook", TEAM_2, sf::Vector2i(-4, 3), PieceDef::Direction::UP);
	addPiece("Rook", TEAM_2, sf::Vector2i(3, 3), PieceDef::Direction::UP);

	// Create knights
	addPiece("Knight", TEAM_1, sf::Vector2i(-3, -4), PieceDef::Direction::DOWN);
	addPiece("Knight", TEAM_1, sf::Vector2i(2, -4), PieceDef::Direction::DOWN);
	addPiece("Knight", TEAM_2, sf::Vector2i(-3, 3), PieceDef::Direction::UP);
	addPiece("Knight", TEAM_2, sf::Vector2i(2, 3), PieceDef::Direction::UP);

	// Create bishops
	addPiece("Bishop", TEAM_1, sf::Vector2i(-2, -4), PieceDef::Direction::DOWN);
	addPiece("Bishop", TEAM_1, sf::Vector2i(1, -4), PieceDef::Direction::DOWN);
	addPiece("Bishop", TEAM_2, sf::Vector2i(-2, 3), PieceDef::Direction::UP);
	addPiece("Bishop", TEAM_2, sf::Vector2i(1, 3), PieceDef::Direction::UP);

	// Create kings
	addPiece("King", TEAM_1, sf::Vector2i(0, -4), PieceDef::Direction::DOWN);
	addPiece("King", TEAM_2, sf::Vector2i(0, 3), PieceDef::Direction::UP);

	// Create queens
	addPiece("Queen", TEAM_1, sf::Vector2i(-1, -4), PieceDef::Direction::DOWN);
	addPiece("Queen", TEAM_2, sf::Vector2i(-1, 3), PieceDef::Direction::UP);

	generateMoveMarkers();
	onCameraChange();
}

/**
 * Update the pieces when the camera changes
 */
void PieceTracker::onCameraChange() {
	for (std::map<sf::Vector2i, Piece*>::iterator i = pieces.begin(); i != pieces.end(); ++i) {
		Piece* piece = i->second;
		piece->onCameraChange(this);
	}
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
bool PieceTracker::addPiece(std::string pieceName, sf::Color team, sf::Vector2i pos, PieceDef::Direction dir) {
	// Check whether a piece is already at the desired location
	if (pieces.find(pos) != pieces.end()) return false;

	std::map<std::string, const PieceDef*>* pieceDefs = game->resourceLoader->pieceDefs;

	// Check whether a piece with the desired name exists
	std::map<std::string, const PieceDef*>::iterator it = pieceDefs->find(pieceName);
	if (it == pieceDefs->end()) return false;

	// Create a copy from the definition
	pieces.insert(std::make_pair(pos, new Piece(it->second, team, pos, dir)));

	return true;
}

/**
 * Remove a piece from the piece tracker
 */
bool PieceTracker::removePiece(sf::Vector2i pos) {
	Piece* piece = getPiece(pos);

	if (piece == nullptr) {
		return false;
	}

	pieces.erase(pieces.find(pos));
	return true;
}

 /**
  * Get the piece at a certain spot
  */
Piece* PieceTracker::getPiece(sf::Vector2i pos) {
	std::map<sf::Vector2i, Piece*>::iterator it = pieces.find(pos);

	// Return the null pointer if the piece is not in the map
	if (it == pieces.end()) {
		return nullptr;
	}

	return it->second;
}

/**
 * Move the piece from one position to another
 *
 * @param pos1 the position of the piece to move
 * @param pos2 the position to move the piece to
 *
 * @return false if there is no piece at pos1, true otherwise
 */
bool PieceTracker::movePiece(MoveMarker* dest) {
	sf::Vector2i pos1 = dest->getRootPiece()->getPos();
	sf::Vector2i pos2 = dest->getPos();
	std::map<sf::Vector2i, Piece*>::iterator it1 = pieces.find(pos1);
	std::map<sf::Vector2i, Piece*>::iterator it2 = pieces.find(pos2);

	// Check if there is a piece to move
	if (it1 == pieces.end()) {
		return false;
	}

	// Get the game piece to move
	Piece* piece = it1->second;

	// Remove the piece at the final destination
	if (it2 != pieces.end()) {
		delete it2->second;
		pieces.erase(it2);
	}

	// Update the position of the piece
	pieces.erase(it1);
	pieces.insert(std::make_pair(pos2, piece));

	// Update the piece
	piece->move(dest);
	piece->onCameraChange(this);

	// Update the move markers for leaving
	std::vector<MoveMarker*>* leavePos = getMoveMarkers(pos1);
	for (std::vector<MoveMarker*>::iterator i = leavePos->begin(); i != leavePos->end(); ++i) {
		(*i)->onPieceLeave(piece, this);
	}

	// Update the move markers for entering
	std::vector<MoveMarker*>* enterPos = getMoveMarkers(pos2);
	for (std::vector<MoveMarker*>::iterator i = enterPos->begin(); i != enterPos->end(); ++i) {
		(*i)->onPieceEnter(piece, this);
	}

	// Clean up
	delete leavePos;
	delete enterPos;
	leavePos = nullptr;
	enterPos = nullptr;

	return true;
}

/**
 * Get the first valid move marker for the position
 */
MoveMarker* PieceTracker::getValidMove(Piece* piece, sf::Vector2i dest) {
	const std::vector<MoveMarker*>* markers = piece->getMoveTracker()->getMoveMarkers(dest);
	MoveMarker* validMove = nullptr;

	for (std::vector<MoveMarker*>::const_iterator i = markers->begin(); i != markers->end(); ++i) {
        if ((*i)->canMove(this)) {
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
		>::const_iterator i = pieces.begin(); i != pieces.end(); ++i
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
