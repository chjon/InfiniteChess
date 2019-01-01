#ifndef CHESS_RESOURCE_LOADER_H
#define CHESS_RESOURCE_LOADER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

// Forward declarations
class GamePiece;
class PieceTracker;



// Class declaration
class ResourceLoader {
private:
	// Constants
    const std::string PIECE_DEFS_FILE = "res/pieces.def";

	// Members
	PieceTracker* pieceTracker;
	std::map<std::string, GamePiece*>* pieceDefs;

	// Utility methods
    void loadPieceDefs(std::string fileName);

public:
	// Constructors
	ResourceLoader(PieceTracker* p);
	~ResourceLoader();

	// Event handlers
	void onStartUp();

    // Friends
    friend PieceTracker;
};

#endif // CHESS_RESOURCE_LOADER_H
