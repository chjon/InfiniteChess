#include "moveDef.h"

#include "moveMarker.h"
#include "piece.h"
#include "pieceDef.h"
#include "vectorUtils.h"

// Public constructors

/**
 * Constructor
 */
MoveDef::MoveDef(
	int index_, sf::Vector2i baseVector_, bool attacksFriendlies_, bool attacksEnemies_,
	bool movesEmpty_, bool canLeap_, bool endsTurn_, bool isXSymmetric_, bool isYSymmetric_, bool isXYSymmetric_,
	const std::vector<int>* chainedMoves_, const std::vector<NumRule*>* scalingRules_,
	const std::vector<NumRule*>* nthStepRules_
) :
	index{index_},
	baseVector{baseVector_},
	attacksFriendlies{attacksFriendlies_},
	attacksEnemies{attacksEnemies_},
    movesEmpty{movesEmpty_},
    canLeap{canLeap_},
    endsTurn{endsTurn_},
    isXSymmetric{isXSymmetric_},
    isYSymmetric{isYSymmetric_},
    isXYSymmetric{isXYSymmetric_},
    chainedMoves{chainedMoves_},
    scalingRules{scalingRules_},
    nthStepRules{nthStepRules_},
    targettingRules{nullptr}
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

	sf::Vector2i transformation = baseVector;

	for (int x = (isXSymmetric ? 0 : 1); x < 2; x++) {
		for (int y = (isYSymmetric ? 0 : 1); y < 2; y++) {
			for (int xy = (isXYSymmetric ? 0 : 1); xy < 2; xy++) {
				// Rotate the vector to the correct direction
				sf::Vector2i rotated = transformation;
				if (piece->dir == PieceDef::Direction::UP) {
					VectorUtils::rotate180(rotated);
				} else if (piece->dir == PieceDef::Direction::LEFT) {
					VectorUtils::rotate90(rotated);
				} else if (piece->dir == PieceDef::Direction::RIGHT) {
					VectorUtils::rotate270(rotated);
				}

				// Add move markers for the current set of transformations
				sf::Vector2i nextPos = piece->pos + rotated;
				markers->push_back(new MoveMarker(piece, this, rotated, nextPos));

				// Swap x and y if the move is xy-symmetric to get the next base vector
				if (isXYSymmetric) {
					std::swap(transformation.x, transformation.y);
				}
			}

			// Invert y if the move is y-symmetric to get the next base vector
			if (isYSymmetric) {
				transformation.y *= -1;
			}
		}

		// Invert x if the move is x-symmetric to get the next base vector
		if (isXSymmetric) {
			transformation.x *= -1;
		}
	}

	return markers;
}
