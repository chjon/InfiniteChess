#include <map>
#include "game.h"
#include "pieceTracker.h"
#include "pieceMove.h"
#include "moveMarker.h"

// Private utility methods

/**
 * Create the move markers for each piece
 */
void PieceTracker::generateMoveMarkers() {
	// Generate and add the move markers for each piece
	for (std::map<sf::Vector2i, GamePiece*>::iterator it = pieces.begin(); it != pieces.end(); ++it) {
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
    for (std::map<sf::Vector2i, GamePiece*>::iterator it = pieces.begin(); it != pieces.end(); it++) {
        delete it->second;
    }

    pieceDefs = nullptr;
}

// Public event handlers

/**
 * Handle the initial loading of pieces
 */
void PieceTracker::onStartup() {
	// Create pawns
    for (int i = 0; i < 8; i++) {
		addPiece("Pawn", sf::Color::White, sf::Vector2i(i, 1), GamePiece::Direction::DOWN);
		addPiece("Pawn", sf::Color::Green, sf::Vector2i(i, 6), GamePiece::Direction::UP);
    }

    // Create rooks
    addPiece("Rook", sf::Color::White, sf::Vector2i(0, 0), GamePiece::Direction::DOWN);
    addPiece("Rook", sf::Color::White, sf::Vector2i(7, 0), GamePiece::Direction::DOWN);
    addPiece("Rook", sf::Color::Green, sf::Vector2i(0, 7), GamePiece::Direction::UP);
    addPiece("Rook", sf::Color::Green, sf::Vector2i(7, 7), GamePiece::Direction::UP);

    // Create knights
    addPiece("Knight", sf::Color::White, sf::Vector2i(1, 0), GamePiece::Direction::DOWN);
    addPiece("Knight", sf::Color::White, sf::Vector2i(6, 0), GamePiece::Direction::DOWN);
    addPiece("Knight", sf::Color::Green, sf::Vector2i(1, 7), GamePiece::Direction::UP);
    addPiece("Knight", sf::Color::Green, sf::Vector2i(6, 7), GamePiece::Direction::UP);

    // Create bishops
    addPiece("Bishop", sf::Color::White, sf::Vector2i(2, 0), GamePiece::Direction::DOWN);
    addPiece("Bishop", sf::Color::White, sf::Vector2i(5, 0), GamePiece::Direction::DOWN);
    addPiece("Bishop", sf::Color::Green, sf::Vector2i(2, 7), GamePiece::Direction::UP);
    addPiece("Bishop", sf::Color::Green, sf::Vector2i(5, 7), GamePiece::Direction::UP);

    // Create kings
    addPiece("King", sf::Color::White, sf::Vector2i(4, 0), GamePiece::Direction::DOWN);
    addPiece("King", sf::Color::Green, sf::Vector2i(4, 7), GamePiece::Direction::UP);

    // Create queens
    addPiece("Queen", sf::Color::White, sf::Vector2i(3, 0), GamePiece::Direction::DOWN);
    addPiece("Queen", sf::Color::Green, sf::Vector2i(3, 7), GamePiece::Direction::UP);

    generateMoveMarkers();
}

/**
 * Update the pieces when the cmaera changes
 */
void PieceTracker::onCameraChange() {
    for (std::map<sf::Vector2i, GamePiece*>::iterator i = pieces.begin(); i != pieces.end(); ++i) {
        GamePiece* piece = i->second;
        piece->onCameraChange();
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
bool PieceTracker::addPiece(std::string pieceName, sf::Color team, sf::Vector2i pos, GamePiece::Direction dir) {
	// Check whether a piece is already at the desired location
	if (pieces.find(pos) != pieces.end()) return false;

	std::map<std::string, GamePiece*>* pieceDefs = game->resourceLoader->pieceDefs;

	// Check whether a piece with the desired name exists
	std::map<std::string, GamePiece*>::iterator it = pieceDefs->find(pieceName);
	if (it == pieceDefs->end()) return false;

	// Create a copy from the definition
	pieces.insert(std::make_pair(pos, new GamePiece(this, it->second, team, pos, dir)));

	return true;
}

/**
 * Remove a piece from the piece tracker
 */
bool PieceTracker::removePiece(sf::Vector2i pos) {
	GamePiece* piece = getPiece(pos);

	if (piece == nullptr) {
		return false;
	}

    pieces.erase(pieces.find(pos));
    return true;
}

 /**
  * Get the piece at a certain spot
  */
GamePiece* PieceTracker::getPiece(sf::Vector2i pos) {
    std::map<sf::Vector2i, GamePiece*>::iterator it = pieces.find(pos);

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
	std::map<sf::Vector2i, GamePiece*>::iterator it1 = pieces.find(pos1);
	std::map<sf::Vector2i, GamePiece*>::iterator it2 = pieces.find(pos2);

	// Check if there is a piece to move
	if (it1 == pieces.end()) {
		return false;
	}

	// Get the game piece at pos1
	GamePiece* piece = it1->second;

	// Remove the piece at the final destination
	if (it2 != pieces.end()) {
		pieces.erase(it2);
	}

	// Update the position of the piece
	pieces.insert(std::make_pair(pos2, piece));
	pieces.erase(it1);

	return true;
}
