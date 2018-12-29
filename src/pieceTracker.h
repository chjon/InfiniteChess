#ifndef CHESS_PIECE_TRACKER_H
#define CHESS_PIECE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "gamePiece.h"

// Forward declarations
class Renderer;



// Class declaration
class PieceTracker {
private:
    // Comparator
    struct cmpVectorLexicographically {
        bool operator()(const sf::Vector2i a, const sf::Vector2i b) const {
            return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
        }
    };

    // Members
    std::map<sf::Vector2i, GamePiece*, cmpVectorLexicographically> pieces;

    // Friends
    friend Renderer;

public:
    // Constructors
    PieceTracker();
    ~PieceTracker();

    // Event handlers
    void onStartup();

    // Methods
    bool addPiece(GamePiece* piece);
    bool removePiece(sf::Vector2i pos);
    GamePiece* getPiece(sf::Vector2i pos);
};

#endif // CHESS_PIECE_TRACKER_H
