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
		throw IOException("Unable to open file: " + fileName);
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

				// Reset stuff
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
				throw FileFormatException("Line must start with a quotation");
			}

			// Check whether the file is formatted properly
			unsigned int i = line.find("\"", 1);
			if (i == std::string::npos) {
                throw FileFormatException("Unmatched quotation");
			}

			// Set the name of the piece and initialize the move set
			pieceName = line.substr(1, i - 1);
			moveSet = new std::vector<PieceMove*>();

			// Check whether a piece with the desired name exists
			std::map<std::string, GamePiece*>::iterator it = pieceDefs->find(pieceName);
			if (it != pieceDefs->end()) {
				throw FileFormatException("Duplicate definition of " + pieceName);
			}

			// Check whether the file is formatted properly
			if (line[line.length() - 1] != '{') {
				throw FileFormatException("Beginning of list not found");
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
