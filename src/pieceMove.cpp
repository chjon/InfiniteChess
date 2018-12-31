#include "pieceMove.h"

// Public constructors

PieceMove::PieceMove(sf::Vector2i baseVector_) :
	isAttack{true},
	allowScaling{false},
	canLeap{false},
    isXSymmetric{false},
    isYSymmetric{false},
    isXYSymmetric{false},
    baseVector{baseVector_}
{
}

PieceMove::~PieceMove() {
}



// Public methods

bool PieceMove::canMove(GamePiece* piece, const sf::Vector2i newPos) {
	sf::Vector2i transformation = baseVector;

	for (int x = (isXSymmetric ? 0 : 1); x < 2; x++) {
		for (int y = (isYSymmetric ? 0 : 1); y < 2; y++) {
			for (int xy = (isXYSymmetric ? 0 : 1); xy < 2; xy++) {
                sf::Vector2i rotated;

                switch (piece->dir) {
				case GamePiece::Direction::UP:
                    rotated.x = -transformation.x;
                    rotated.y = -transformation.y;
                    break;

				case GamePiece::Direction::LEFT:
					rotated.x = -transformation.y;
					rotated.y = transformation.x;
					break;

				case GamePiece::Direction::RIGHT:
					rotated.x = transformation.y;
					rotated.y = -transformation.x;
					break;

				// Down is default
				default:
					rotated.x = transformation.x;
					rotated.y = transformation.y;
					break;
                }

				if (newPos == (piece->pos + rotated)) {
					return true;
				}

				if (!isXYSymmetric) {
					break;
				} else {
					std::swap(transformation.x, transformation.y);
				}
			}

			if (!isYSymmetric) {
				break;
			} else {
				transformation.y = -transformation.y;
			};
		}

		if (!isXSymmetric) {
			break;
		} else {
			transformation.x = -transformation.x;
		}
	}

	return false;
}
