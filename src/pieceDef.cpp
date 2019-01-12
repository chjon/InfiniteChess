#include "pieceDef.h"
#include "pieceMove.h"

//Constructors

/**
 * Constructor
 */
PieceDef::PieceDef(const std::string name_, const bool isCheckVulnerable_, const bool isRoyal_, const std::map<int, const PieceMove*>* moves_) :
	name{name_},
	isCheckVulnerable{isCheckVulnerable_},
	isRoyal{isRoyal_},
	moves{moves_}
{
}

/**
 * Destructor
 */
PieceDef::~PieceDef() {
	// Delete each of the stored moves
	for (std::map<int, const PieceMove*>::const_iterator i = moves->begin(); i != moves->end(); ++i) {
		delete i->second;
	}

	delete moves;
	moves = nullptr;
}
