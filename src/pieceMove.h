#ifndef CHESS_PIECE_MOVE_H
#define CHESS_PIECE_MOVE_H

#include <SFML/Graphics.hpp>
#include "gamePiece.h"
#include <vector>

// Forward declarations
class GamePiece;
class PieceTracker;



// Class declaration
class PieceMove {
private:
	// Members
	PieceTracker* pieceTracker;

	// Friends
	friend GamePiece;
	friend PieceTracker;
	friend MoveMarker;
	friend ResourceLoader;

public:
	// Members
	const int index;

	const sf::Vector2i baseVector;

	const bool attacksFriendlies;
	const bool attacksEnemies;
	const bool movesEmpty;
	const bool canLeap;
	const bool endsTurn;
	const bool isXSymmetric;
	const bool isYSymmetric;
	const bool isXYSymmetric;

	const std::vector<int> chainedMoves;
	const std::vector<int> scalingRules;
	const std::vector<int> nthStepRules;
	const std::vector<int> targettingRules;

	// Constructors
	PieceMove(
		PieceTracker* p, int index_, sf::Vector2i baseVector_, bool attacksFriendlies_, bool attacksEnemies_,
		bool movesEmpty_, bool canLeap, bool endsTurn, bool isXSymmetric, bool isYSymmetric, bool isXYSymmetric
	);

	~PieceMove();

	// Methods
	void generateMoveMarkers(GamePiece* piece);
};

#endif // CHESS_PIECE_MOVE_H
