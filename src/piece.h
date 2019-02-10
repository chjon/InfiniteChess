#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <SFML/Graphics.hpp>
#include "pieceDef.h"
#include "vectorUtils.h"

// Forward declarations
class Event;
class MoveMarker;
class MoveTracker;
class PieceDef;
class PieceMove;
class PieceTracker;
class Renderer;
class TargetingRule;



// Class declaration
class Piece {
private:
	// Piece definition
    const PieceDef* pieceDef;

    // Piece properties
    unsigned int team;
    sf::Vector2i pos;
	PieceDef::Direction dir;
	unsigned int moveCount;
	int lastMove;

	// Move tracker
	MoveTracker* moveTracker;

	// Friends
	friend Renderer;
	friend PieceMove;
	friend MoveDef;

public:
	// Constructors
	Piece(
		const PieceDef* pieceDef_,
		const unsigned int team_,
		sf::Vector2i pos_,
		PieceDef::Direction dir_,
		const unsigned int moveCount_,
		const int lastMove_
	);
	~Piece();

	// Accessors
	inline const sf::Vector2i getPos() const { return pos; }
	inline const unsigned int getTeam() const { return team; }
	inline const PieceDef::Direction getDir() const { return dir; }
	inline const unsigned int getMoveCount() const { return moveCount; }
	inline const PieceDef* getDef() const { return pieceDef; }
	inline const MoveTracker* getMoveTracker() const { return moveTracker; }
	inline const int getLastMove() const { return lastMove; }
	const MoveMarker* getValidMove(sf::Vector2i pos) const;
	inline const std::string toString() const {
        return "[" +
			pieceDef->name + "," +
			std::to_string(team) + "," +
			VectorUtils::toString(pos) + "," +
			std::to_string(dir) + "," +
			std::to_string(moveCount) + "," +
			std::to_string(lastMove) + "," +
		"]";
	}

	const std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>* getTargets(sf::Vector2i pos) const;

	// Mutators
	void setPos(sf::Vector2i dest);
	inline void setLastMove(int index) { lastMove = index; }
	void move(sf::Vector2i dest);

	// Event handlers
	void onStartUp(PieceTracker* pieceTracker);
	void onCameraChange(PieceTracker* pieceTracker);
	void onMove();
};

#endif // CHESS_PIECE_H
