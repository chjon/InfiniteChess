#include <fstream>
#include <string>
#include "resourceLoader.h"
#include "stringUtils.h"
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

// Object generation

/**
 * Get a list of objects from a string
 *
 * @param listString the string from which to generate the list of objects
 * @param objectFromString a function for generating an object from a string
 * @param T the type of object to stored in the returned list
 *
 * @return the list of objects represented by the string
 */
template <typename T> std::vector<T>* ResourceLoader::getListFromString(
	const std::string& listString,
	T(*objectFromString)(const std::string& s)
) {
	// Validate input
	// checkBracketEnclosed(listString);

	// Instantiate the list
	std::vector<T>* objectList = new std::vector<T>();
	const std::vector<std::string>* objectStrings =	StringUtils::getList(
		listString.substr(1, listString.length() - 2), ',', '[', ']'
	);

	// Load objects
	for (std::vector<std::string>::const_iterator i = objectStrings->begin(); i != objectStrings->end(); ++i) {
		objectList->push_back(objectFromString(*i));
	}

	// Clean up and return
	delete objectStrings;
	return objectList;
}

/**
 * Get a map of objects from a string
 *
 * @param listString the string from which to generate the map of objects
 * @param keyFromObject a function for generating a key from an object
 * @param objectFromString a function for generating an object from a string
 * @param K the key type by which to index the objects
 * @param V the type of object to be stored in the map
 *
 * @return the map of objects represented by the string
 */
template <typename K, typename V> std::map<K, V>* ResourceLoader::getMapFromString(
	const std::string& listString,
	K(*keyFromObject)(V val),
	V(*objectFromString)(const std::string& s)
) {
	// Validate input
	// checkBracketEnclosed(listString);

	// Instantiate the map
	std::map<K, V>* objectMap = new std::map<K, V>();
	const std::vector<std::string>* objectStrings = StringUtils::getList(
		listString.substr(1, listString.length() - 2), ',', '[', ']'
	);

	// Load objects
	for (std::vector<std::string>::const_iterator i = objectStrings->begin(); i != objectStrings->end(); ++i) {
		// Create the key-value pair
		V val = objectFromString(*i);
		K key = keyFromObject(val);

		// Validate the key
		if (objectMap->find(key) != objectMap->end()) {
			throw ResourceLoader::FileFormatException("Duplicate definition for key");
		}

		// Add the key-value pair
		objectMap->insert(std::make_pair(key, val));
	}

	// Clean up and return
	delete objectStrings;
	return objectMap;
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
