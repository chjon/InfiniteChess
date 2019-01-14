#include <map>
#include "game.h"
#include "pieceTracker.h"
#include "moveMarker.h"
#include "piece.h"
#include <iostream>

// Private utility methods

/**
 * Create the move markers for each piece
 */
void PieceTracker::generateMoveMarkers() {
	// Generate and add the move markers for each piece
	for (std::map<sf::Vector2i, Piece*>::iterator it = pieces.begin(); it != pieces.end(); ++it) {
		it->second->onMove();
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
	for (int i = 0; i < 8; i++) {
		addPiece("Pawn", TEAM_1, sf::Vector2i(i, 1), PieceDef::Direction::DOWN);
		addPiece("Pawn", TEAM_2, sf::Vector2i(i, 6), PieceDef::Direction::UP);
	}

	// Create rooks
	addPiece("Rook", TEAM_1, sf::Vector2i(0, 0), PieceDef::Direction::DOWN);
	addPiece("Rook", TEAM_1, sf::Vector2i(7, 0), PieceDef::Direction::DOWN);
	addPiece("Rook", TEAM_2, sf::Vector2i(0, 7), PieceDef::Direction::UP);
	addPiece("Rook", TEAM_2, sf::Vector2i(7, 7), PieceDef::Direction::UP);

	// Create knights
	addPiece("Knight", TEAM_1, sf::Vector2i(1, 0), PieceDef::Direction::DOWN);
	addPiece("Knight", TEAM_1, sf::Vector2i(6, 0), PieceDef::Direction::DOWN);
	addPiece("Knight", TEAM_2, sf::Vector2i(1, 7), PieceDef::Direction::UP);
	addPiece("Knight", TEAM_2, sf::Vector2i(6, 7), PieceDef::Direction::UP);

	// Create bishops
	addPiece("Bishop", TEAM_1, sf::Vector2i(2, 0), PieceDef::Direction::DOWN);
	addPiece("Bishop", TEAM_1, sf::Vector2i(5, 0), PieceDef::Direction::DOWN);
	addPiece("Bishop", TEAM_2, sf::Vector2i(2, 7), PieceDef::Direction::UP);
	addPiece("Bishop", TEAM_2, sf::Vector2i(5, 7), PieceDef::Direction::UP);

	// Create kings
	addPiece("King", TEAM_1, sf::Vector2i(4, 0), PieceDef::Direction::DOWN);
	addPiece("King", TEAM_2, sf::Vector2i(4, 7), PieceDef::Direction::UP);

	// Create queens
	addPiece("Queen", TEAM_1, sf::Vector2i(3, 0), PieceDef::Direction::DOWN);
	addPiece("Queen", TEAM_2, sf::Vector2i(3, 7), PieceDef::Direction::UP);

	generateMoveMarkers();
}

/**
 * Update the pieces when the cmaera changes
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
bool PieceTracker::movePiece(sf::Vector2i pos1, sf::Vector2i pos2) {
	std::map<sf::Vector2i, Piece*>::iterator it1 = pieces.find(pos1);
	std::map<sf::Vector2i, Piece*>::iterator it2 = pieces.find(pos2);

	// Check if there is a piece to move
	if (it1 == pieces.end()) {
		return false;
	}

	// Get the game piece at pos1
	Piece* piece = it1->second;

	// Remove the piece at the final destination
	if (it2 != pieces.end()) {
		pieces.erase(it2);
	}

	// Update the position of the piece
	pieces.insert(std::make_pair(pos2, piece));
	pieces.erase(it1);

	return true;
}
