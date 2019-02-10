#ifndef CHESS_CONTROLLER_H
#define CHESS_CONTROLLER_H

#include <SFML/Graphics.hpp>
#include "component_trackers/actionListenerTracker.h"
#include "component_trackers/eventProcessor.h"
#include "game.h"

// Forward declarations
class ActionListenerTracker;
class Game;
class InputHandler;
class MoveMarker;
class PieceTracker;
class Piece;

// Helper structs
struct TeamNode {
	unsigned int teamIndex;
    TeamNode* next;
    unsigned int numPieces;
    std::string name;
    sf::Color color;
};



// Class declaration
class Controller {
private:
	// Members
	Game* game;
	PieceTracker* pieceTracker;
	ActionListenerTracker actionListenerTracker;
	EventProcessor eventProcessor;

	std::map<unsigned int, TeamNode*> teams;
	TeamNode* curTurn;
	Piece* selectedPiece;

	// Event handlers
	void onMousePress (sf::Vector2i pos);

	// Friends
	friend InputHandler;

	// Helpers
	void clearTeams();
	void move(const MoveMarker* dest);
	void advanceTurn();
	inline std::string colorToString(sf::Color color) const {
		return "[" +
			std::to_string(color.r) + "," +
			std::to_string(color.g) + "," +
			std::to_string(color.b) + "," +
			std::to_string(color.a) + "," +
		"]";
	}
	inline std::string teamToString(TeamNode* team) const {
		return "[" +
			std::to_string(team->teamIndex) + "," +
			team->name + "," +
			colorToString(team->color) + "," +
		"]";
	}

public:
	// Constructors
	Controller(Game* g, PieceTracker* p);
	~Controller();

	// Event handlers
	void onStartup(
		std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams,
		unsigned int curTeam
	);
	void onGeneration(MoveMarker* marker);

	// Accessors
	Piece* getSelectedPiece() const;
	bool canMove(unsigned int team) const;
	inline std::string teamsToString() const {
		std::string output = "[";
		TeamNode* curNode = curTurn;
		do {
            output += teamToString(curNode) + ",\n";
            curNode = curNode->next;
		} while (curNode != curTurn);

		return output + "]";
	}
	inline unsigned int getCurTurn() const { return curTurn->teamIndex; };

	// Mutators
	inline void addPiece(unsigned int teamIndex) {
		std::map<unsigned int, TeamNode*>::iterator i = teams.find(teamIndex);
		if (i != teams.end()) {
            i->second->numPieces++;
		}
	}

	void removePiece(unsigned int teamIndex) {
		std::map<unsigned int, TeamNode*>::iterator i = teams.find(teamIndex);
		if (i != teams.end()) {
            i->second->numPieces--;
		}
	}
};

#endif // CHESS_CONTROLLER_H
