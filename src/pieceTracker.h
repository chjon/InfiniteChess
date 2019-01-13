#ifndef CHESS_PIECE_TRACKER_H
#define CHESS_PIECE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "pieceDef.h"
#include "vectorUtils.h"

// Forward declarations
class Game;
class Renderer;
class PieceDef;
class Piece;



// Class declaration
class PieceTracker {
private:
    // Members
	Game* game;

    std::map<std::string, PieceDef*>* pieceDefs;
    std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically> pieces;

    // Utility methods

    /**
     * Create the move markers for each piece
     */
    void generateMoveMarkers();

    // Friends
    friend Renderer;

public:
    // Constructors
    PieceTracker(Game* g);
    ~PieceTracker();

    // Event handlers
    void onStartup();
    void onCameraChange();

    // Accessors

    /**
     * Determine whether a certain position is within the bounds of the screen
     */
	bool isRenderable(sf::Vector2i pos) const;

    // Methods
    bool addPiece(std::string name, sf::Color team, sf::Vector2i pos, PieceDef::Direction);
    bool removePiece(sf::Vector2i pos);
    Piece* getPiece(sf::Vector2i pos);
    bool movePiece(sf::Vector2i pos1, sf::Vector2i pos2);
};

#endif // CHESS_PIECE_TRACKER_H
