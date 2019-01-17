#include "moveTracker.h"

#include "moveMarker.h"
#include "piece.h"
#include "pieceDef.h"
#include "moveDef.h"

// Private methods

/**
 * Delete all of the piece's move markers
 */
void MoveTracker::clearMarkers() {
	// Remove all the markers from the list of terminal move markers
	terminalMoveMarkers->clear();

	// Delete the markers
	for (std::map<
			const MoveDef*,
			std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>*
		>::iterator i = moveMarkers->begin(); i != moveMarkers->end(); ++i
	) {
		for (std::map<
				sf::Vector2i,
				MoveMarker*,
				VectorUtils::cmpVectorLexicographically
			>::iterator j = i->second->begin(); j != i->second->end(); ++j
		) {
            delete j->second;
		}

		i->second->clear();
    }
}

/**
 * Generate the piece's move markers
 */
void MoveTracker::generateMarkers() {
	// Generate the initial move markers for each move
	for (std::map<
			const MoveDef*,
			std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>*
		>::iterator i = moveMarkers->begin(); i != moveMarkers->end(); ++i
	) {
		// Add the initial move markers for the move
		const std::vector<MoveMarker*>* markers = i->first->generateMarkers(piece);
		for (std::vector<MoveMarker*>::const_iterator j = markers->begin(); j != markers->end(); ++j) {
			i->second->insert(std::make_pair((*j)->getPos(), *j));
            terminalMoveMarkers->push_back(*j);
		}

		delete markers;
		markers = nullptr;
    }
}



// Constructors

/**
 * Constructor
 */
MoveTracker::MoveTracker(Piece* piece_) :
	piece{piece_},
    moveMarkers{new std::map<
		const MoveDef*,
		std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>*
	>()},
	terminalMoveMarkers{new std::vector<MoveMarker*>()}
{
	// Create a map for each move
	const std::map<int, const MoveDef*>* moves = piece->getDef()->moves;
    for (std::map<int, const MoveDef*>::const_iterator i = moves->begin(); i != moves->end(); ++i) {
        moveMarkers->insert(std::make_pair(
			i->second,
			new std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>()
		));
    }
}

/**
 * Destructor
 */
MoveTracker::~MoveTracker() {
	// Delete all move markers
	clearMarkers();

	// Delete the stored maps
    for (std::map<
			const MoveDef*,
			std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>*
		>::iterator i = moveMarkers->begin(); i != moveMarkers->end(); ++i
	) {
		delete i->second;
    }

    delete moveMarkers;
    delete terminalMoveMarkers;

    moveMarkers = nullptr;
    terminalMoveMarkers = nullptr;
}



// Event handlers

/**
 * Update the move markers when the camera changes
 */
void MoveTracker::onCameraChange(PieceTracker* pieceTracker) {
    std::vector<MoveMarker*> temp;

	// Update the terminal markers
	for (std::vector<MoveMarker*>::iterator i = terminalMoveMarkers->begin(); i != terminalMoveMarkers->end(); ++i) {
        MoveMarker* terminal = *i;

        // Extend the move marker upward
        while (pieceTracker->isRenderable(terminal->getNextPos())) {
			const MoveDef* rootMove = terminal->getRootMove();
			MoveMarker* prev = terminal;
			terminal = new MoveMarker(piece, rootMove, terminal->getBaseVector(), terminal->getNextPos());

			// Link the move markers
			prev->setNext(terminal);

			// Check if a leap is necessary for the move
			if (prev->getRequiresLeap() || pieceTracker->getPiece(prev->getPos()) != nullptr) {
				terminal->setRequiresLeap(true);
			}
            moveMarkers->find(rootMove)->second->insert(std::make_pair(terminal->getPos(), terminal));
        }

        // Add the new terminal move marker to a temporary list
        temp.push_back(terminal);
	}

	// Copy all the new terminal move markers to the terminal move marker list
	terminalMoveMarkers->clear();
	for (MoveMarker* marker : temp) {
		terminalMoveMarkers->push_back(marker);
	}
}

/**
 * Update the move markers when the piece moves
 */
void MoveTracker::onMove() {
	// Clear all of the piece's old move markers
	clearMarkers();

	// Generate new move markers
	generateMarkers();
}



// Methods

/**
 * Get all of the piece's move markers at a certain position
 */
const std::vector<MoveMarker*>* MoveTracker::getMoveMarkers(sf::Vector2i pos) const {
	std::vector<MoveMarker*>* markers = new std::vector<MoveMarker*>();

	// Add all of the move markers at the given position
	for (std::map<
			const MoveDef*,
			std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>*
		>::iterator i = moveMarkers->begin(); i != moveMarkers->end(); ++i
	) {
        // Check if the move has a marker at this position
        std::map<sf::Vector2i, MoveMarker*>::iterator found = i->second->find(pos);
        if (found != i->second->end()) {
            markers->push_back(found->second);
        }
    }

    return markers;
}
