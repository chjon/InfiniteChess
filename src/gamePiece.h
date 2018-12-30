#ifndef CHESS_GAME_PIECE_H
#define CHESS_GAME_PIECE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Forward declarations
class Renderer;
class PieceTracker;
class Controller;
class PieceMove;



// Class declaration
class GamePiece {
private:
	// Members
	std::string name = "GamePiece";
	sf::Color team   = sf::Color::White;
    sf::Vector2i pos;
    unsigned int moveCount;
    std::vector<PieceMove*> moveSet;

    // Friends
    friend Renderer;
    friend PieceTracker;
    friend Controller;
    friend PieceMove;

public:
	// Constructors
	GamePiece();
	GamePiece(
		std::string name_,
		sf::Color team_,
		sf::Vector2i pos,
		unsigned int moveCount_
	);

	~GamePiece();

	// Mutators
	void addMove(PieceMove* newMove);

	// Methods
	bool canMove (const sf::Vector2i newPos);
	void move (const sf::Vector2i newPos);
};

#endif // CHESS_GAME_PIECE_H
