#include <map>
#include "pieceTracker.h"
#include "pieceMove.h"

// Public constructors

/**
 * Constructor
 */
PieceTracker::PieceTracker() {
}

/**
 * Destructor
 */
PieceTracker::~PieceTracker() {
    // Delete all the stored pieces
    for (std::map<sf::Vector2i, GamePiece*>::iterator it = pieces.begin(); it != pieces.end(); it++) {
        delete it->second;
    }
}

// Public event handlers

/**
 * Handle the initial loading of pieces
 */
void PieceTracker::onStartup() {
	PieceMove* newMove = new PieceMove(sf::Vector2i(0, 1));

    for (int i = 0; i < 8; i++) {
		GamePiece* piece = new GamePiece("Pawn", sf::Color::White, sf::Vector2i(i, 1), 0);
		piece->addMove(newMove);
        addPiece(piece);

        piece = new GamePiece("Pawn", sf::Color::Green, sf::Vector2i(i, 6), 0);
		piece->addMove(newMove);
        addPiece(piece);
    }
}



// Public methods

/**
 * Add a piece to the piece tracker
 */
bool PieceTracker::addPiece(GamePiece* piece) {
    return pieces.insert(std::make_pair(piece->pos, piece)).second;
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
