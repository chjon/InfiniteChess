#include <iostream>
#include <fstream>
#include <string>
#include "resourceLoader.h"
#include "stringUtils.h"
#include "pieceMove.h"
#include "pieceDefLoader.h"

// Private utility methods

/**
 * Load all the piece definitions
 */
void ResourceLoader::loadPieceDefs(std::string fileName) {
	PieceDefLoader::loadPieceDefs(fileName);
}

// Public constructors

/**
 * Constructor
 */
ResourceLoader::ResourceLoader(PieceTracker* p) :
	pieceTracker{p},
	pieceDefs{new std::map<std::string, GamePiece*>()},
	textures{new std::map<std::string, sf::Texture*>()}
{
}

/**
 * Destructor
 */
ResourceLoader::~ResourceLoader() {
	// Delete all the textures
    for (std::map<std::string, sf::Texture*>::iterator it = textures->begin(); it != textures->end(); it++) {
		if (it->second != nullptr) {
			delete it->second;
		}
    }

	// Delete all the definitions
    for (std::map<std::string, GamePiece*>::iterator it = pieceDefs->begin(); it != pieceDefs->end(); it++) {
		it->second->definitionDelete();
		delete it->second;
    }

    delete textures;
    delete pieceDefs;

    textures     = nullptr;
    pieceDefs    = nullptr;
    pieceTracker = nullptr;
}



// Public event handlers

/**
 * Initialize everything
 */
void ResourceLoader::onStartUp() {
	loadPieceDefs(PIECE_DEFS_FILE);
}
