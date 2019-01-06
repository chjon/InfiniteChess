#ifndef CHESS_GAME_PIECE_H
#define CHESS_GAME_PIECE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "vectorUtils.h"

// Forward declarations
class Renderer;
class PieceTracker;
class Controller;
class PieceMove;
class ResourceLoader;
class MoveMarker;



// Class declaration
class GamePiece {
public:
	enum Direction {
		UP, DOWN, LEFT, RIGHT
	};

private:
	// Members
	const std::string name;
	const std::vector<PieceMove*>* moveSet;
	const sf::Color team;

    sf::Vector2i pos;
    Direction dir;
    unsigned int moveCount;

    /**
	 * The piece's move markers
	 */
	std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically> moveMarkers;

    // Utility methods
    void definitionDelete();

    /**
     * Delete the piece's current move markers
     */
	void deleteMoveMarkers();

    // Friends
    friend Renderer;
    friend PieceTracker;
    friend Controller;
    friend PieceMove;
    friend ResourceLoader;

public:
	// Constructors
	GamePiece(const std::string n, const std::vector<PieceMove*>* m);
	GamePiece(const GamePiece* piece, const sf::Color team_, sf::Vector2i pos_, Direction dir_);
	~GamePiece();

	// Utility methods

	/**
	 * Update the piece's move markers
	 */
	void updateMoveMarkers();

	bool canMove (const sf::Vector2i newPos);
	void move (const sf::Vector2i newPos);
};

#endif // CHESS_GAME_PIECE_H
