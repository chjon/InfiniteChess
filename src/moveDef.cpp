#include "moveDef.h"

#include "moveMarker.h"
#include "piece.h"
#include "pieceDef.h"
#include "vectorUtils.h"

// Private helper methods

/**
 * Rotate a vector
 */
sf::Vector2i MoveDef::rotate(const sf::Vector2i original, const PieceDef::Direction dir) {
	sf::Vector2i rotated = original;
	if (dir == PieceDef::Direction::UP) {
		VectorUtils::rotate180(rotated);
	} else if (dir == PieceDef::Direction::LEFT) {
		VectorUtils::rotate90(rotated);
	} else if (dir == PieceDef::Direction::RIGHT) {
		VectorUtils::rotate270(rotated);
	}

	return rotated;
}



// Public constructors

/**
 * Constructor
 */
MoveDef::MoveDef(
	int index_, sf::Vector2i baseVector_, bool canLeap_, bool endsTurn_, bool isXSymmetric_, bool isYSymmetric_, bool isXYSymmetric_,
	const std::vector<int>* chainedMoves_, const std::vector<NumRule*>* scalingRules_,
	const std::vector<NumRule*>* nthStepRules_, const std::vector<const TargetingRule*>* targetingRules_
) :
	index{index_},
	baseVector{baseVector_},
    canLeap{canLeap_},
    endsTurn{endsTurn_},
    isXSymmetric{isXSymmetric_},
    isYSymmetric{isYSymmetric_},
    isXYSymmetric{isXYSymmetric_},
    chainedMoves{chainedMoves_},
    scalingRules{scalingRules_},
    nthStepRules{nthStepRules_},
    targetingRules{targetingRules_}
{
}

/**
 * Destructor
 */
MoveDef::~MoveDef() {
	delete chainedMoves;

	chainedMoves = nullptr;
}



// Public methods

/**
 * Generate the initial move markers
 */
const std::vector<MoveMarker*>* MoveDef::generateMarkers(const Piece* piece) const {
	std::vector<MoveMarker*>* markers = new std::vector<MoveMarker*>();

	// Rotate the vector to the correct direction
	sf::Vector2i rotated = rotate(baseVector, piece->dir);

	for (int x = (isXSymmetric ? 0 : 1); x < 2; x++) {
		for (int y = (isYSymmetric ? 0 : 1); y < 2; y++) {
			for (int xy = (isXYSymmetric ? 0 : 1); xy < 2; xy++) {
                sf::Vector2i reflected = VectorUtils::reflect(rotated, !x, !y, !xy);

				// Add move markers for the current set of transformations
				sf::Vector2i nextPos = piece->pos + reflected;
				markers->push_back(new MoveMarker(piece, this, reflected, nextPos, !x, !y, !xy, 1));
			}
		}
	}

	return markers;
}
