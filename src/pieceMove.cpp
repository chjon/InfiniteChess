#include "pieceMove.h"
#include "vectorUtils.h"
#include "pieceTracker.h"
#include "moveMarker.h"

// Public constructors

/**
 * Constructor
 */
PieceMove::PieceMove(
	PieceTracker* p, int index_, sf::Vector2i baseVector_, bool attacksFriendlies_, bool attacksEnemies_,
	bool movesEmpty_, bool canLeap_, bool endsTurn_, bool isXSymmetric_, bool isYSymmetric_, bool isXYSymmetric_
) :
	pieceTracker{p},
	index{index_},
	baseVector{baseVector_},
	attacksFriendlies{attacksFriendlies_},
	attacksEnemies{attacksEnemies_},
    movesEmpty{movesEmpty_},
    canLeap{canLeap_},
    endsTurn{endsTurn_},
    isXSymmetric{isXSymmetric_},
    isYSymmetric{isYSymmetric_},
    isXYSymmetric{isXYSymmetric_}
{
}

/**
 * Destructor
 */
PieceMove::~PieceMove() {
	pieceTracker = nullptr;
}



// Public methods

/**
 * Generate the move markers for this move
 */
void PieceMove::generateMoveMarkers(GamePiece* piece) {
	sf::Vector2i transformation = baseVector;

	for (int x = (isXSymmetric ? 0 : 1); x < 2; x++) {
		for (int y = (isYSymmetric ? 0 : 1); y < 2; y++) {
			for (int xy = (isXYSymmetric ? 0 : 1); xy < 2; xy++) {
				// Rotate the vector to the correct direction
				sf::Vector2i rotated = transformation;
				if (piece->dir == GamePiece::Direction::UP) {
					VectorUtils::rotate180(rotated);
				} else if (piece->dir == GamePiece::Direction::LEFT) {
					VectorUtils::rotate90(rotated);
				} else if (piece->dir == GamePiece::Direction::RIGHT) {
					VectorUtils::rotate270(rotated);
				}

				// Add move markers for the current set of transformations
				sf::Vector2i nextPos = piece->pos + rotated;
				MoveMarker* newMoveMarker = new MoveMarker(piece, this, rotated, nextPos);
				piece->moveMarkers.insert(std::make_pair(nextPos, newMoveMarker));

				/*/ Add the terminal move markers
				if (allowScaling) {
					piece->terminalMoveMarkers.push_back(newMoveMarker);
				}*/

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
}
