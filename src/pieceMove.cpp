#include "pieceMove.h"
#include "vectorUtils.h"
#include "pieceTracker.h"
#include "moveMarker.h"

// Private utility methods

bool PieceMove::canMove(GamePiece::Direction dir, const sf::Vector2i base, const sf::Vector2i candidate) {
	// Rotate the vector to the correct direction
	sf::Vector2i rotated = base;
	if (dir == GamePiece::Direction::UP) {
		VectorUtils::rotate180(rotated);
	} else if (dir == GamePiece::Direction::LEFT) {
		VectorUtils::rotate90(rotated);
	} else if (dir == GamePiece::Direction::RIGHT) {
		VectorUtils::rotate270(rotated);
	}

	// Check if the displacement matches the vector
	return (candidate == rotated) ||
		(allowScaling && VectorUtils::isIntegerMultiple(rotated, candidate));
}



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
				MoveMarker* newMoveMarker;
				sf::Vector2i nextPos = piece->pos + rotated;

				do {
					// Check whether the next position meets the attack requirements
					GamePiece* pieceAtLocation = pieceTracker->getPiece(nextPos);
					if ((moveType == MoveType::MOVE_ONLY   && (pieceAtLocation != nullptr)) ||
						(moveType == MoveType::ATTACK_ONLY && (pieceAtLocation == nullptr))
					) {
						if (canLeap) {
							continue;
						} else {
							break;
						}
					}

					newMoveMarker = new MoveMarker(piece, this, rotated, nextPos);
					piece->moveMarkers.insert(std::make_pair(nextPos, newMoveMarker));
					nextPos = newMoveMarker->getNextPos();
				} while (allowScaling && pieceTracker->isRenderable(nextPos));

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

bool PieceMove::canMove(GamePiece* piece, const sf::Vector2i newPos) {
	GamePiece* pieceAtLocation = pieceTracker->getPiece(newPos);

	// Check whether the new position meets the attack requirements
	if ((moveType == MoveType::MOVE_ONLY   && (pieceAtLocation != nullptr)) ||
		(moveType == MoveType::ATTACK_ONLY && (pieceAtLocation == nullptr))
	) {
		return false;
	}

	const sf::Vector2i displacement = newPos - piece->pos;
	sf::Vector2i transformation = baseVector;

	for (int x = (isXSymmetric ? 0 : 1); x < 2; x++) {
		for (int y = (isYSymmetric ? 0 : 1); y < 2; y++) {
			for (int xy = (isXYSymmetric ? 0 : 1); xy < 2; xy++) {
				// Check if the current set of transformations matches the desired position
				if (canMove(piece->dir, transformation, displacement)) {
					return true;
				}

				// Swap x and y if the move is x-y symmetric
				if (isXYSymmetric) {
					std::swap(transformation.x, transformation.y);
				}
			}

			// Invert y if the move is y symmetric
			if (isYSymmetric) {
				transformation.y *= -1;
			}
		}

		// Invert x if the move is x symmetric
		if (isXSymmetric) {
			transformation.x *= -1;
		}
	}

	return false;
}
