#ifndef CHESS_PIECE_TRACKER_H
#define CHESS_PIECE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "gamePiece.h"
#include "vectorUtils.h"

// Forward declarations
class Renderer;



// Class declaration
class PieceTracker {
private:
    // Members

    std::map<std::string, GamePiece*> pieceDefs;
    std::map<sf::Vector2i, GamePiece*, VectorUtils::cmpVectorLexicographically> pieces;

    // Friends
    friend Renderer;

public:
    // Constructors
    PieceTracker();
    ~PieceTracker();

    // Event handlers
    void onStartup();

    // Methods
    bool definePiece(const std::string name, const std::vector<PieceMove*>* moveSet);
    bool addPiece(std::string name, sf::Color team, sf::Vector2i pos);
    bool removePiece(sf::Vector2i pos);
    GamePiece* getPiece(sf::Vector2i pos);
    bool movePiece(sf::Vector2i pos1, sf::Vector2i pos2);
};

#endif // CHESS_PIECE_TRACKER_H
