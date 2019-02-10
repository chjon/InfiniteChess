#ifndef CHESS_RESOURCE_LOADER_H
#define CHESS_RESOURCE_LOADER_H

#include <fstream>
#include <SFML/Graphics.hpp>
#include "../utils/stringUtils.h"

// Forward declarations
class BoardLoader;
class PieceDefLoader;

// Class declaration
class ResourceLoader {
private:
	// Utility methods

    /**
	 * Determine the number of arguments in the string
	 */
	inline static const unsigned int getNumArgs(const std::string& s) {
		// Check whether the string is empty
		if (s.empty()) {
			return 0;
		}

		// Count the number of arguments
		unsigned int nestLevel = 0;
		unsigned int argCount = 0;

		for (unsigned int i = 0; i < s.length(); ++i) {
			char curChar = s[i];

			if (curChar == BRACKET_OPEN) {
				nestLevel++;
			} else if (curChar == BRACKET_CLOSE) {
				nestLevel--;
			} else if (nestLevel == 0 && curChar == SEPARATOR) {
				argCount++;
			}
		}

		return argCount;
	}

	// Validation methods
    /**
	 * Determine whether a file name is valid. A file name is valid if it ends with
	 * the correct extension
	 *
	 * @param fileName the candidate file name
	 *
	 * @return true if the file name is valid, false otherwise
	 */
	inline static const bool isValidFileName(const std::string& fileName, const std::string& extension) {
		// Check whether the filename has the required length
		if (fileName.length() < extension.length()) {
			return false;
		}

		// Check whether the file ends with the correct extension
		for (unsigned int i = 1; i <= extension.length(); i++) {
			if (extension[extension.length() - i] != fileName[fileName.length() - i]) {
				return false;
			}
		}

		return true;
	}

    /**
	 * Check whether the string is bracket enclosed
	 */
	inline static const void checkBracketEnclosed(const std::string& s) {
		// Check whether the string is bracket-enclosed
		if (s[0] != BRACKET_OPEN || s[s.length() - 1] != BRACKET_CLOSE) {
			throw FileFormatException("Expected bracket-enclosed string");
		}
	}

    /**
	 * Check whether the string contains the expected number of arguments
	 */
	inline static const void checkNumArgs(const std::string& s, unsigned int expected) {
		unsigned int numMoveArgs = getNumArgs(s);
		if (numMoveArgs != expected) {
			throw FileFormatException(
				"Expecting " + std::to_string(expected) + " arguments, " +
				"received " + std::to_string(numMoveArgs) + ": " + s
			);
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
	template <typename T> inline static std::vector<T>* getListFromString(
		const std::string& listString,
		T(*objectFromString)(const std::string&)
	) {
		// Validate input
		checkBracketEnclosed(listString);

		// Instantiate the list
		std::vector<T>* objectList = new std::vector<T>();
		const std::vector<std::string>* objectStrings =	StringUtils::getList(
			listString.substr(1, listString.length() - 2), SEPARATOR, BRACKET_OPEN, BRACKET_CLOSE
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
	template <typename K, typename V> inline static std::map<K, V>* getMapFromString(
		const std::string& listString,
		K(*keyFromObject)(V),
		V(*objectFromString)(const std::string&)
	) {
		// Validate input
		checkBracketEnclosed(listString);

		// Instantiate the map
		std::map<K, V>* objectMap = new std::map<K, V>();
		const std::vector<std::string>* objectStrings = StringUtils::getList(
			listString.substr(1, listString.length() - 2), SEPARATOR, BRACKET_OPEN, BRACKET_CLOSE
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

	// I/O Methods
	/**
	 * Read a file into a single string, removing whitespace
	 */
	inline static const std::string removeWhiteSpace(const std::string& fileName) {
		std::ifstream file(fileName, std::ios::in);
		std::string line;

		// Check whether the file is open
		if (!file.is_open()) {
			throw ResourceLoader::IOException("Unable to open file: " + fileName);
		}

		// Read each line of the file into a single string
		std::string compressedString = "";
		while (std::getline(file, line)) {
			unsigned int beginIndex = 0;
			for (unsigned int i = 0; i < line.length(); i++) {
				// Skip the rest of the line if it is commented out
				if (line[i] == ResourceLoader::COMMENT_MARKER) {
					compressedString += line.substr(beginIndex, i - beginIndex);
					beginIndex = line.length();
					break;
				}

				// Skip whitespace
				if (line[i] == ' ' || line[i] == '\t') {
					if (beginIndex != i) {
						compressedString += line.substr(beginIndex, i - beginIndex);
					}

					beginIndex = i + 1;
				}
			}

			compressedString += line.substr(beginIndex);
		}

		// Close the file
		file.close();

		return compressedString;
	}

	// Friends
	friend BoardLoader;
	friend PieceDefLoader;

public:
	// Constants
	static const char BRACKET_OPEN   = '[';
	static const char BRACKET_CLOSE  = ']';
	static const char SEPARATOR      = ',';
	static const char COMMENT_MARKER = '#';

	// Exceptions

	class FileFormatException : public std::exception {
	private:
		const std::string message;
	public:
		// Constructors
		inline FileFormatException(const std::string m) : message{m} {}
		inline ~FileFormatException() {}

		// Overridden methods
		inline virtual const char* what() const throw() {
			return message.c_str();
		}
	};

	class IOException : public std::exception {
	private:
		const std::string message;
	public:
		// Constructors
		inline IOException(const std::string m) : message{m} {}
		inline ~IOException() {}

		// Overridden methods
		inline virtual const char* what() const throw() {
			return message.c_str();
		}
	};

	/**
	 * Get a list of objects from the entries of a map
	 *
	 * @param originMap the map from which to generate the list
	 * @param typeFromPair a function for generating list elements from map entries
	 *
	 * @return the list of objects generated from the map
	 */
	template <typename T, typename K, typename V> inline static const std::vector<T>* getListFromMap(
        const std::map<K, V>* originMap,
        T(*typeFromPair)(K, V)
	) {
		std::vector<T>* outputList = new std::vector<T>();
		for (typename std::map<K, V>::const_iterator i = originMap->begin(); i != originMap->end(); ++i) {
            outputList->push_back(typeFromPair(i->first, i->second));
		}

		return outputList;
	}

	/**
	 * Load all the textures
	 */
    inline static std::map<std::string, sf::Texture*>* loadTextures(
		const std::vector<std::string>* fileNames, const std::string& texturesDirectory, const std::string& texturesExtension
	) {
		std::map<std::string, sf::Texture*>* textures = new std::map<std::string, sf::Texture*>();
		for (std::vector<std::string>::const_iterator i = fileNames->begin(); i != fileNames->end(); ++i) {
			sf::Texture* texture = new sf::Texture();
			texture->loadFromFile(texturesDirectory + *i + texturesExtension);
			textures->insert(std::make_pair(*i, texture));
		}
		return textures;
    }

    /**
     * Write to file
     */
    inline static void write(std::string fileName, std::string toWrite) {
		std::ofstream file(fileName, std::ios::out);

		// Check whether the file is open
		if (!file.is_open()) {
			throw ResourceLoader::IOException("Unable to open file: " + fileName);
		}

		file << toWrite;

		// Close the file
		file.close();
    }
};

#endif // CHESS_RESOURCE_LOADER_H
