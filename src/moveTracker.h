#ifndef CHESS_MOVE_TRACKER_H
#define CHESS_MOVE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "vectorUtils.h"

// Forward declarations
class Piece;
class MoveDef;
class MoveMarker;
class Renderer;
class PieceTracker;



/**
 * This class keeps track of a piece's move markers
 */
class MoveTracker {
private:
	// References
	Piece* piece;

	// Members
	std::map<const MoveDef*, std::map<
		sf::Vector2i,
		MoveMarker*,
		VectorUtils::cmpVectorLexicographically>*
	>* moveMarkers;

	std::vector<MoveMarker*>* terminalMoveMarkers;

	// Helper methods
    void clearMarkers();
    void generateMarkers();

	// Friends
	friend Piece;
	friend Renderer;

public:
    // Constructors
    MoveTracker(Piece* piece_);
    ~MoveTracker();

    // Event handlers
    void onCameraChange(PieceTracker* pieceTracker);
	void onMove();

    // Methods
    std::vector<MoveMarker*>* getMoveMarkers(sf::Vector2i pos);
};

#endif // CHESS_MOVE_TRACKER_H
