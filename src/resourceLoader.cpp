#include <iostream>
#include <fstream>
#include <string>
#include "resourceLoader.h"
#include "stringUtils.h"
#include "pieceMove.h"
#include "pieceDefLoader.h"
#include "pieceDef.h"

// Private utility methods

/**
 * Load all the piece definitions
 */
void ResourceLoader::loadPieceDefs(std::string fileName) {
	pieceDefs = PieceDefLoader::loadPieceDefs(fileName);
}

/**
 * Load all the textures
 */
void ResourceLoader::loadTextures() {
	textures = new std::map<std::string, sf::Texture*>();

    for (std::map<std::string, const PieceDef*>::iterator i = pieceDefs->begin(); i != pieceDefs->end(); ++i) {
        sf::Texture* texture = new sf::Texture();
        texture->loadFromFile(TEXTURES_DIRECTORY + i->second->name + TEXTURES_EXTENSION);
        textures->insert(std::make_pair(i->second->name, texture));
    }
}

// Public constructors

/**
 * Constructor
 */
ResourceLoader::ResourceLoader(PieceTracker* p) :
	pieceTracker{p},
	pieceDefs{nullptr},
	textures{nullptr}
{
}

/**
 * Destructor
 */
ResourceLoader::~ResourceLoader() {
	// Delete all the textures
	if (textures != nullptr) {
		for (std::map<std::string, sf::Texture*>::iterator it = textures->begin(); it != textures->end(); it++) {
			if (it->second != nullptr) {
				delete it->second;
			}
		}

		delete textures;
		textures = nullptr;
	}


	// Delete all the definitions
	if (pieceDefs != nullptr) {
		for (std::map<std::string, const PieceDef*>::iterator it = pieceDefs->begin(); it != pieceDefs->end(); it++) {
			delete it->second;
		}

		delete pieceDefs;
		pieceDefs = nullptr;
	}

    pieceTracker = nullptr;
}



// Public event handlers

/**
 * Initialize everything
 */
void ResourceLoader::onStartUp() {
	loadPieceDefs(PIECE_DEFS_FILE);
	loadTextures();
}
