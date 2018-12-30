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
    return pieces.erase(pieces.find(pos)) != pieces.end();
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
