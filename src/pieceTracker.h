#ifndef CHESS_PIECE_TRACKER_H
#define CHESS_PIECE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "gamePiece.h"

// Class definition

class PieceTracker {
private:
    // Members
    std::map<sf::Vector2i, GamePiece*, std::map::key_compare> pieces;

public:
    // Constructors
    PieceTracker();
    ~PieceTracker();

    // Methods
    bool addPiece(GamePiece* piece);
    bool removePiece(sf::Vector2i pos);
    GamePiece* getPiece(sf::Vector2i pos);
};

#endif // CHESS_PIECE_TRACKER_H
