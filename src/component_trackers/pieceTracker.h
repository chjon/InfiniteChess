#ifndef CHESS_PIECE_TRACKER_H
#define CHESS_PIECE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "../components/pieceDef.h"
#include "../utils/vectorUtils.h"

// Forward declarations
class Game;
class MoveMarker;
class Piece;
class PieceDef;
class Renderer;



// Class declaration
class PieceTracker {
private:
    // Members
	Game* game;

    std::map<std::string, const PieceDef*>* pieceDefs;
    std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>* pieces;

    // Friends
    friend Renderer;

public:
    // Constructors
    PieceTracker(Game* g);
    ~PieceTracker();

    // Event handlers
    void onStartup(
		std::map<std::string, const PieceDef*>* defs,
		std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>* startPieces
	);
    void onCameraChange();
	void onGeneration(MoveMarker* generated);

    // Accessors

    const std::vector<Piece*>* getPieces() const;
    Piece* getPiece(sf::Vector2i pos) const;

    /**
     * Determine whether a certain position is within the bounds of the screen
     */
	bool shouldGenerate(const MoveMarker* terminal) const;
	bool shouldDelete(const MoveMarker* terminal) const;

    // Methods
    void clearPieces();
    void addPiece(Piece* piece);
    bool removePiece(sf::Vector2i pos);
    const MoveMarker* getValidMove(Piece* piece, sf::Vector2i dest);
    std::vector<MoveMarker*>* getMoveMarkers(sf::Vector2i pos) const;

    std::string piecesToString() const;
};

#endif // CHESS_PIECE_TRACKER_H
