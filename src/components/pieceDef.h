#ifndef CHESS_PIECE_DEF_H
#define CHESS_PIECE_DEF_H

#include <string>
#include <map>

// Forward declarations
class MoveDef;
class PieceDefLoader;



// Class declaration
class PieceDef {
public:
	enum Direction {
		UP, DOWN, LEFT, RIGHT
	};

	// Members
	const std::string name;
	const bool isCheckVulnerable;
	const bool isRoyal;

	const std::map<int, const MoveDef*>* moves;

	// Constructors
	PieceDef(const std::string name_, const bool isCheckVulnerable_, const bool isRoyal_, const std::map<int, const MoveDef*>* moves);
	~PieceDef();
};

#endif // CHESS_PIECE_DEF_H
