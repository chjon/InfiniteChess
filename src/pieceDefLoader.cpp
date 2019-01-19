#include "pieceDefLoader.h"

#include <fstream>
#include <iostream>
#include "moveDef.h"
#include "numRule.h"
#include "pieceDef.h"
#include "resourceLoader.h"

// Initialize constants
const std::string PieceDefLoader::EXTENSION = ".def";
const std::vector<char>* PieceDefLoader::WHITESPACE = new std::vector<char>(' ', '\t');



// Private methods

/**
 * Determine whether a file name is valid. A file name is valid if it ends with
 * the correct extension
 *
 * @param fileName the candidate file name
 *
 * @return true if the file name is valid, false otherwise
 */
const bool PieceDefLoader::isValidFileName(const std::string& fileName) {
	// Check whether the filename has the required length
	if (fileName.length() < EXTENSION.length()) {
		return false;
	}

	// Check whether the file ends with the correct extension
	for (unsigned int i = 1; i <= EXTENSION.length(); i++) {
		if (EXTENSION[EXTENSION.length() - i] != fileName[fileName.length() - i]) {
			return false;
		}
	}

	return true;
}

/**
 * Create a piece definition from a string
 *
 * @param pieceString the string from which to generate the piece definition
 *
 * @return a piece definition generated from the string
 *
 * @throw ResourceLoader::FileFormatException if the piece string is not bracket-enclosed,
 *        if the piece name cannot be found, or if the move set cannot be found
 */
const PieceDef* PieceDefLoader::pieceDefFromString(const std::string& pieceString) {
	// Validate input
	checkBracketEnclosed(pieceString);
    checkNumArgs(pieceString.substr(1, pieceString.length() - 2), 3);

	const std::vector<std::string>* properties = split(pieceString.substr(1, pieceString.length() - 2));

	// Get the name
	unsigned int propertyIndex = 0;
	const std::string pieceName = (*properties)[propertyIndex++];

	// Get piece properties
	unsigned int index = 0;
	const bool isCheckVulnerable = (*properties)[propertyIndex][index++] - '0';
	const bool isRoyal = (*properties)[propertyIndex][index++] - '0';
	propertyIndex++;

	// Get move set
	const std::map<int, const MoveDef*>* moveSet = getMovesFromString((*properties)[propertyIndex++]);

	// Clean up and return
	delete properties;
    return new PieceDef(pieceName, isCheckVulnerable, isRoyal, moveSet);
}

/**
 * Create a move set from a string
 *
 * @param movesString the string from which to generate the move set
 *
 * @return a map containing all of the piece's possible moves
 */
const std::map<int, const MoveDef*>* PieceDefLoader::getMovesFromString(const std::string& movesString) {
	// Validate input
	checkBracketEnclosed(movesString);

	// Generate the move set
	std::map<int, const MoveDef*>* moves = new std::map<int, const MoveDef*>();
	const std::vector<std::string>* moveStrings = split(movesString.substr(1, movesString.length() - 2));

	// Load integers
	for (std::vector<std::string>::const_iterator i = moveStrings->begin(); i != moveStrings->end(); ++i) {
		const MoveDef* moveDef = getMoveFromString(*i);
		moves->insert(std::make_pair(moveDef->index, moveDef));
	}

	// Clean up and return
	delete moveStrings;
	return moves;
}

/**
 * Create a move from a string
 *
 * @param the string from which to generate the move
 *
 * @return a move generated from the string
 */
const MoveDef* PieceDefLoader::getMoveFromString(const std::string& moveString) {
	// Validate input
	checkBracketEnclosed(moveString);
	checkNumArgs(moveString.substr(1, moveString.length() - 2), NUM_MOVE_ARGS);

	// Get all arguments
	const std::vector<std::string>* args = split(moveString.substr(1, moveString.length() - 2));
	unsigned int argIndex = 0;

	const int moveIndex = std::stoi((*args)[argIndex++]);
	const sf::Vector2i baseVector = getVectorFromString((*args)[argIndex++]);

    // Get the move properties
    unsigned int index = 0;
    const bool attacksFriendlies = (*args)[argIndex][index++] - '0';
    const bool attacksEnemies    = (*args)[argIndex][index++] - '0';
    const bool movesEmpty        = (*args)[argIndex][index++] - '0';
    const bool canLeap           = (*args)[argIndex][index++] - '0';
    const bool endsTurn          = (*args)[argIndex][index++] - '0';
    const bool isXSymmetric      = (*args)[argIndex][index++] - '0';
    const bool isYSymmetric      = (*args)[argIndex][index++] - '0';
    const bool isXYSymmetric     = (*args)[argIndex][index++] - '0';
    argIndex++;

    // Load chained moves and movement rules
	const std::vector<int>* chainedMoves = getChainedMovesFromString((*args)[argIndex++]);
	const std::vector<NumRule*>* scalingRules = getNumRulesFromString((*args)[argIndex++]);
	const std::vector<NumRule*>* nthStepRules = getNumRulesFromString((*args)[argIndex++]);

	// TODO: Load targetting rules

	MoveDef* newMove = new MoveDef(
		moveIndex, baseVector, attacksFriendlies, attacksEnemies, movesEmpty, canLeap, endsTurn,
		isXSymmetric, isYSymmetric, isXYSymmetric, chainedMoves, scalingRules, nthStepRules
	);

	// Clean up and return
	delete args;
	return newMove;
}

/**
 * Determine the number of arguments in the string
 */
const unsigned int PieceDefLoader::getNumArgs(const std::string& s) {
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

/**
 * Create a vector from a string
 */
const sf::Vector2i PieceDefLoader::getVectorFromString(const std::string& s) {
	// Validate input
	checkBracketEnclosed(s);
	checkNumArgs(s.substr(1, s.length() - 2), 2);

	// Load vector components
	const std::vector<std::string>* strings = split(s.substr(1, s.length() - 2));
	const int x = std::stoi((*strings)[0]);
	const int y = std::stoi((*strings)[1]);

	// Clean up and return
	delete strings;
	return sf::Vector2i(x, y);
}

/**
 * Get a list of integers from a string
 */
const std::vector<int>* PieceDefLoader::getChainedMovesFromString(const std::string& s) {
	// Validate input
	checkBracketEnclosed(s);

	std::vector<int>* chainedMovesList = new std::vector<int>();
	const std::vector<std::string>* strings = split(s.substr(1, s.length() - 2));

	// Load integers
	for (std::vector<std::string>::const_iterator i = strings->begin(); i != strings->end(); ++i) {
		chainedMovesList->push_back(std::stoi(*i));
	}

	// Clean up and return
	delete strings;
	return chainedMovesList;
}

/**
 * Get a list of number rules from a string
 */
const std::vector<NumRule*>* PieceDefLoader::getNumRulesFromString(const std::string& s) {
	// Validate input
	checkBracketEnclosed(s);

	std::vector<NumRule*>* rulesList = new std::vector<NumRule*>();
	const std::vector<std::string>* rulesStrings = split(s.substr(1, s.length() - 2));

	// Load num rules
	for (std::vector<std::string>::const_iterator i = rulesStrings->begin(); i != rulesStrings->end(); ++i) {
		rulesList->push_back(new NumRule(*i));
	}

	// Clean up and return
	delete rulesStrings;
	return rulesList;
}

/**
 * Split the string on the separator while leaving bracket-enclosed groups intact
 */
const std::vector<std::string>* PieceDefLoader::split(const std::string& s) {
	// Read each line of the file
	std::vector<std::string>* splitStrings = new std::vector<std::string>();
	std::string stringSoFar = "";
	int nestLevel = 0;

	// Split the string on the separator
	int lineBegin = 0;
	for (unsigned int i = 0; i < s.length(); i++) {
		const char curChar = s[i];
		if (curChar == BRACKET_OPEN) {
			nestLevel++;
		} else if (curChar == BRACKET_CLOSE) {
			// Check if a bracket was expected
			if (nestLevel == 0) {
				throw ResourceLoader::FileFormatException(
					"Unexpected token '" + std::to_string(BRACKET_CLOSE) + "'"
				);
			}

			nestLevel--;
		} else if (curChar == SEPARATOR && nestLevel == 0) {
			// Split
			stringSoFar += s.substr(lineBegin, i - lineBegin);
			lineBegin = i + 1;
			splitStrings->push_back(stringSoFar);
			stringSoFar = "";
		}
	}

	return splitStrings;
}

/**
 * Read a file into a single string, removing whitespace
 */
const std::string PieceDefLoader::removeWhiteSpace(const std::string& fileName) {
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
			if (line[i] == COMMENT_MARKER) {
				compressedString += line.substr(beginIndex, i - beginIndex);
				beginIndex = line.length();
				break;
			}

			// Skip whitespace
            if (isWhitespace(line[i])) {
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

/**
 * Check whether a character is a whitespace character
 */
const bool PieceDefLoader::isWhitespace(const char candidate) {
    for (std::vector<char>::const_iterator i = WHITESPACE->begin(); i != WHITESPACE->end(); ++i) {
        if (*i == candidate) return true;
    }

    return false;
}

/**
 * Create the piece definition map from a list of piece strings
 */
std::map<std::string, const PieceDef*>* PieceDefLoader::getPieceDefsFromStrings(const std::vector<std::string>* pieceStrings) {
	// Create the pieces from each of the piece strings
	std::map<std::string, const PieceDef*>* pieceDefs = new std::map<std::string, const PieceDef*>();
	for (std::string s : *pieceStrings) {
		const PieceDef* pieceDef = pieceDefFromString(s);

		// Check if the piece definition already exists
		if (pieceDefs->find(pieceDef->name) != pieceDefs->end()) {
			throw ResourceLoader::FileFormatException("Duplicate definition of " + pieceDef->name);
		}

        pieceDefs->insert(std::make_pair(pieceDef->name, pieceDef));
	}

	return pieceDefs;
}


// Validation methods

/**
 * Check whether the string is bracket enclosed
 */
const void PieceDefLoader::checkBracketEnclosed(const std::string& s) {
	// Check whether the string is bracket-enclosed
	if (s[0] != BRACKET_OPEN || s[s.length() - 1] != BRACKET_CLOSE) {
		throw ResourceLoader::FileFormatException("Expected bracket-enclosed string");
	}
}

/**
 * Check whether the string contains the expected number of arguments
 */
const void PieceDefLoader::checkNumArgs(const std::string& s, unsigned int expected) {
	unsigned int numMoveArgs = getNumArgs(s);
	if (numMoveArgs != expected) {
		throw ResourceLoader::FileFormatException(
			"Expecting " + std::to_string(expected) + " arguments, received " + std::to_string(numMoveArgs)
		);
	}
}

// Public methods

/**
 * Load piece definitions from file
 */
std::map<std::string, const PieceDef*>* PieceDefLoader::loadPieceDefs (const std::string& fileName) {
	// Check whether filename is valid
	if (!isValidFileName(fileName)) {
		throw ResourceLoader::FileFormatException("Invalid file name");
	}

	// Create the piece definitions
	const std::vector<std::string>* pieceStrings = split(removeWhiteSpace(fileName));
	std::map<std::string, const PieceDef*>* pieceDefs = getPieceDefsFromStrings(pieceStrings);

	// Clean up and return
	delete pieceStrings;
	return pieceDefs;
}
