#ifndef CHESS_GAME_PIECE_H
#define CHESS_GAME_PIECE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "pieceMove.h"

class GamePiece {
private:
	// Members
    std::vector<PieceMove> moveSet;

public:
	// Members
	std::string name = "GamePiece";
	std::string team = "Team";
    sf::Vector2i pos;
    unsigned int moveCount;

	// Constructors
	GamePiece();
	GamePiece(
		std::string name_,
		std::string team_,
		sf::Vector2i pos,
		unsigned int moveCount_
	);

	~GamePiece();

	// Mutators
	void addMove(PieceMove newMove);

	// Methods
	bool canMove (sf::Vector2i newPos);
	void move (sf::Vector2i newPos);
};

#endif // CHESS_GAME_PIECE_H
