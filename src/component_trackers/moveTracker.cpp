#include "moveTracker.h"

#include "../components/moveMarker.h"
#include "../components/piece.h"
#include "../components/pieceDef.h"
#include "../components/moveDef.h"

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
 * Update the move markers on generation
 */
void MoveTracker::onStartup(PieceTracker* pieceTracker) {
	// Update the move marker on generation
	for (std::vector<MoveMarker*>::iterator i = terminalMoveMarkers->begin(); i != terminalMoveMarkers->end(); ++i) {
		(*i)->onGeneration(pieceTracker);
	}
}

/**
 * Update the move markers when the camera changes
 */
void MoveTracker::onCameraChange(PieceTracker* pieceTracker) {
    std::vector<MoveMarker*> temp;

	// Update the terminal markers
	for (std::vector<MoveMarker*>::iterator i = terminalMoveMarkers->begin(); i != terminalMoveMarkers->end(); ++i) {
        MoveMarker* terminal = *i;

		// Contract the move markers backward
		while (pieceTracker->shouldDelete(terminal)) {
			const MoveDef* rootMove = terminal->getRootMove();
			MoveMarker* toDelete = terminal;
			MoveMarker* prev = const_cast<MoveMarker*>(terminal->getPrev());

			// Unlink the move markers
			prev->setNext(nullptr);
			moveMarkers->find(rootMove)->second->erase(terminal->getPos());

			// Clean up
			delete toDelete;
			toDelete = nullptr;
			terminal = prev;
		}

        // Extend the move marker forward
        while (terminal->getNextPos() != terminal->getPos() && pieceTracker->shouldGenerate(terminal)) {
			const MoveDef* rootMove = terminal->getRootMove();
			MoveMarker* prev = terminal;

			terminal = new MoveMarker(
				piece, rootMove, terminal->getBaseVector(), terminal->getNextPos(),
				terminal->switchedX, terminal->switchedY, terminal->switchedXY, terminal->lambda + 1
			);

			// Link the move markers
			prev->setNext(terminal);
            moveMarkers->find(rootMove)->second->insert(std::make_pair(terminal->getPos(), terminal));

			terminal->setNumObstructions((prev == nullptr) ? (0) : (
				prev->getNumObstructions() + (pieceTracker->getPiece(prev->getPos()) != nullptr)
			));

            // Update the move marker on generation
            terminal->onGeneration(pieceTracker);
            pieceTracker->onGeneration(terminal);
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

const std::vector<MoveMarker*>* MoveTracker::getMoveMarkers() const {
	std::vector<MoveMarker*>* markers = new std::vector<MoveMarker*>();

	// Add all of the move markers
	for (std::map<
			const MoveDef*,
			std::map<sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>*
		>::iterator i = moveMarkers->begin(); i != moveMarkers->end(); ++i
	) {
        for (std::map<sf::Vector2i, MoveMarker*>::iterator j = i->second->begin();
			j != i->second->end(); ++j
		) {
			markers->push_back(j->second);
        }
    }

    return markers;
}

/**
 * Get all the targets for the move markers at a given position
 *
 * @param pos the position of the move markers
 */
const std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>* MoveTracker::getTargets(sf::Vector2i pos) {
	std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>* targets = new std::vector<std::tuple<MoveMarker*, Piece*, const TargetingRule*>>();

	// Add the targets for each move marker
	const std::vector<MoveMarker*>* markers = getMoveMarkers(pos);
	for (std::vector<MoveMarker*>::const_iterator i = markers->begin(); i != markers->end(); ++i) {
		const std::vector<std::pair<Piece*, const TargetingRule*>>* markerTargets = (*i)->getTargets();
		for (std::vector<std::pair<Piece*, const TargetingRule*>>::const_iterator j = markerTargets->begin();
			j != markerTargets->end(); ++j
		) {
            targets->push_back(std::make_tuple(*i, (*j).first, (*j).second));
		}
	}

    return targets;
}
