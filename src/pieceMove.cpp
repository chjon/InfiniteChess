#include "pieceMove.h"
#include "vectorUtils.h"
#include "pieceTracker.h"
#include "moveMarker.h"

// Public constructors

/**
 * Constructor
 */
PieceMove::PieceMove(PieceTracker* p, sf::Vector2i baseVector_) :
	allowScaling{false},
	canLeap{false},
	isXSymmetric{false},
	isYSymmetric{false},
	isXYSymmetric{false},
	pieceTracker{p},
	moveType{MoveType::GENERAL},
	baseVector{baseVector_}
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

				// Add the terminal move markers
				if (allowScaling) {
					piece->terminalMoveMarkers.push_back(newMoveMarker);
				}

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
