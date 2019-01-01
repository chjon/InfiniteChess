#include <iostream>
#include <fstream>
#include <string>
#include "resourceLoader.h"
#include "stringUtils.h"
#include "pieceMove.h"

// Private utility methods

/**
 * Load all the piece definitions
 */
void ResourceLoader::loadPieceDefs(std::string fileName) {
    std::ifstream file(fileName, std::ios::in);
    std::string line;

    // Check whether the file is open
    if (!file.is_open()) {
		std::cout << "Error: Unable to open file." << std::endl;
		return;
    }


    bool foundDef = false;
    std::string pieceName;
    std::vector<PieceMove*>* moveSet;

	while (std::getline(file, line)) {
		StringUtils::trim(line);

		// Skip empty lines
		if (line.empty()) {
			continue;
		}

        if (foundDef) {
			// Check for end of definition
			if (line == "}") {
                // Create piece template
				pieceDefs->insert(std::make_pair(pieceName, new GamePiece(pieceName, moveSet)));
				std::cout << pieceName << ": " << moveSet->size() << " moves loaded" << std::endl;
				moveSet = nullptr;

				foundDef = false;
				continue;
			}

			std::vector<std::string> moveParams = StringUtils::split(line, ",");

			// Read base vector
			sf::Vector2i baseVector = sf::Vector2i(
                std::stoi(moveParams[1]),
                std::stoi(moveParams[2])
			);

			// Create move
			PieceMove* newMove = new PieceMove(pieceTracker, baseVector);

			unsigned int i = 0;
			unsigned int j = 0;

			// Set move type
			newMove->moveType = (PieceMove::MoveType)(((char) moveParams[i][j++]) - '0');

			// Read flags
			newMove->allowScaling  = (((char) moveParams[i][j++]) - '0');
			newMove->canLeap       = (((char) moveParams[i][j++]) - '0');
			newMove->isXSymmetric  = (((char) moveParams[i][j++]) - '0');
			newMove->isYSymmetric  = (((char) moveParams[i][j++]) - '0');
			newMove->isXYSymmetric = (((char) moveParams[i][j++]) - '0');

			// Add move to move set
			moveSet->push_back(newMove);
			newMove = nullptr;

        } else {
        	// Check if the piece is declared properly
			if (line[0] != '"') {
				std::cout << "Error: Line must start with a quotation" << std::endl;
				return;
			}

			unsigned int i = line.find("\"", 1);
			if (i == std::string::npos) {
                std::cout << "Error: Unmatched quotation" << std::endl;
				return;
			}

			pieceName = line.substr(1, i - 1);
			moveSet = new std::vector<PieceMove*>();

			// Check whether a piece with the desired name exists
			std::map<std::string, GamePiece*>::iterator it = pieceDefs->find(pieceName);
			if (it != pieceDefs->end()) {
				std::cout << "Error: Duplicate definition of " << pieceName << std::endl;
				return;
			}

			if (line[line.length() - 1] != '{') {
				std::cout << "Error: Beginning of list not found" << std::endl;
				return;
			}

			foundDef = true;
        }
	}

    file.close();
}

// Public constructors

/**
 * Constructor
 */
ResourceLoader::ResourceLoader(PieceTracker* p) :
	pieceTracker{p},
	pieceDefs{new std::map<std::string, GamePiece*>()}
{
}

/**
 * Destructor
 */
ResourceLoader::~ResourceLoader() {
	pieceTracker = nullptr;

	// Delete all the definitions
    for (std::map<std::string, GamePiece*>::iterator it = pieceDefs->begin(); it != pieceDefs->end(); it++) {
		it->second->definitionDelete();
		delete it->second;
    }

    delete pieceDefs;
}



// Public event handlers

/**
 * Initialize everything
 */
void ResourceLoader::onStartUp() {
	loadPieceDefs(PIECE_DEFS_FILE);
}
