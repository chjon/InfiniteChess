#ifndef CHESS_MOVE_TRACKER_H
#define CHESS_MOVE_TRACKER_H

#include <SFML/Graphics.hpp>
#include <map>
#include "../utils/vectorUtils.h"

// Forward declarations
class Event;
class MoveDef;
class MoveMarker;
class Piece;
class PieceTracker;
class Renderer;
class TargetingRule;



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
    void onStartup(PieceTracker* pieceTracker);
    void onCameraChange(PieceTracker* pieceTracker);
	void onMove();

    // Methods
    const std::vector<MoveMarker*>* getMoveMarkers(sf::Vector2i pos) const;
    const std::vector<MoveMarker*>* getMoveMarkers() const;
	const std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>* getTargets(sf::Vector2i pos);
};

#endif // CHESS_MOVE_TRACKER_H
