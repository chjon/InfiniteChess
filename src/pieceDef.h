#ifndef CHESS_PIECE_DEF_H
#define CHESS_PIECE_DEF_H

#include <string>
#include <map>

// Forward declarations
class PieceMove;
class PieceDefLoader;



// Class declaration
class PieceDef {
public:
	// Members
	const std::string name;
	const bool isCheckVulnerable;
	const bool isRoyal;

	const std::map<int, const PieceMove*>* moves;

	// Constructors
	PieceDef(const std::string name_, const bool isCheckVulnerable_, const bool isRoyal_, const std::map<int, const PieceMove*>* moves);
	~PieceDef();
};

#endif // CHESS_PIECE_DEF_H
