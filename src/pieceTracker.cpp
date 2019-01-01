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

    // Delete all the definitions
    for (std::map<std::string, GamePiece*>::iterator it = pieceDefs.begin(); it != pieceDefs.end(); it++) {
		it->second->definitionDelete();
		delete it->second;
    }
}

// Public event handlers

/**
 * Handle the initial loading of pieces
 */
void PieceTracker::onStartup() {
	// Create move set for pawns
	std::vector<PieceMove*>* pawnMoveSet = new std::vector<PieceMove*>();

	// Create pawn step
	PieceMove* pawnStep = new PieceMove(this, sf::Vector2i(0, 1));
	pawnStep->moveType = PieceMove::MoveType::MOVE_ONLY;
	pawnMoveSet->push_back(pawnStep);
	pawnStep = new PieceMove(this, sf::Vector2i(1, 1));
	pawnStep->moveType = PieceMove::MoveType::ATTACK_ONLY;
	pawnStep->isXSymmetric = true;
	pawnMoveSet->push_back(pawnStep);
	pawnStep = nullptr;

	// Create template pawn
	pieceDefs.insert(std::make_pair("Pawn", new GamePiece("Pawn", pawnMoveSet)));
	pawnMoveSet = nullptr;

	// Create move set for queens
	std::vector<PieceMove*>* queenMoveSet = new std::vector<PieceMove*>();

	// Create queen step
	PieceMove* queenStep = new PieceMove(this, sf::Vector2i(0, 1));
	queenStep->isXSymmetric = true;
	queenStep->isXYSymmetric = true;
	queenStep->allowScaling = true;
	queenMoveSet->push_back(queenStep);
	queenStep = new PieceMove(this, sf::Vector2i(1, 1));
	queenStep->isXSymmetric = true;
	queenStep->isYSymmetric = true;
	queenStep->allowScaling = true;
	queenMoveSet->push_back(queenStep);

	queenStep = nullptr;

	// Create template queen
	pieceDefs.insert(std::make_pair("Queen", new GamePiece("Queen", queenMoveSet)));
	queenMoveSet = nullptr;

	// Create pawns
    for (int i = 0; i < 8; i++) {
		addPiece("Pawn", sf::Color::White, sf::Vector2i(i, 1), GamePiece::Direction::DOWN);
		addPiece("Pawn", sf::Color::Green, sf::Vector2i(i, 6), GamePiece::Direction::UP);
    }

    // Create queens
    addPiece("Queen", sf::Color::White, sf::Vector2i(3,0), GamePiece::Direction::DOWN);
    addPiece("Queen", sf::Color::Green, sf::Vector2i(3,7), GamePiece::Direction::UP);
}



// Public methods

/**
 * Create a template piece
 */
bool PieceTracker::definePiece(const std::string name, const std::vector<PieceMove*>* moveSet) {
	// Check whether a piece with the desired name exists
    std::map<std::string, GamePiece*>::iterator it = pieceDefs.find(name);
    if (it == pieceDefs.end()) return false;

    // Add the piece to the definitions
    pieceDefs.insert(std::make_pair(name, new GamePiece(name, moveSet)));
    return true;
}

/**
 * Add a piece to the piece tracker
 */
bool PieceTracker::addPiece(std::string pieceName, sf::Color team, sf::Vector2i pos, GamePiece::Direction dir) {
	// Check whether a piece is already at the desired location
	if (pieces.find(pos) != pieces.end()) return false;

	// Check whether a piece with the desired name exists
	std::map<std::string, GamePiece*>::iterator it = pieceDefs.find(pieceName);
	if (it == pieceDefs.end()) return false;

	// Create a copy from the definition
	pieces.insert(std::make_pair(pos, new GamePiece(it->second, team, pos, dir)));

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
