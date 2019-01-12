#include "pieceDefLoader.h"
#include "pieceDef.h"
#include <iostream>
#include <fstream>
#include "resourceLoader.h"
#include "pieceMove.h"


// Private constants

const std::string PieceDefLoader::EXTENSION = ".def";



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
	// Check whether the piece string is bracket-enclosed
	if (pieceString[0] != BRACKET_OPEN || pieceString[pieceString.length() - 1] != BRACKET_CLOSE) {
		throw ResourceLoader::FileFormatException("Expected bracket-enclosed piece definition");
	}

	// Get the name
	unsigned int index = 0;
	while (index < pieceString.length() && pieceString[index] != SEPARATOR) {
        index++;
	}

	if (index == pieceString.length()) {
		throw ResourceLoader::FileFormatException("Failed to find piece name");
	}

	const std::string pieceName = pieceString.substr(1, index++ - 1);

	// Get piece properties
	const bool isCheckVulnerable = pieceString[index++] - '0';
	const bool isRoyal = pieceString[index++] - '0';

	// Get move set
	if (pieceString[index++] != SEPARATOR) {
		throw ResourceLoader::FileFormatException("Failed to find moveset");
	}

	const std::map<int, const PieceMove*>* moveSet = getMovesFromString(
		pieceString.substr(index, pieceString.length() - index - 1)
	);

    return new PieceDef(pieceName, isCheckVulnerable, isRoyal, moveSet);
}

/**
 * Create a move set from a string
 *
 * @param movesString the string from which to generate the move set
 *
 * @return a map containing all of the piece's possible moves
 */
const std::map<int, const PieceMove*>* PieceDefLoader::getMovesFromString(const std::string& movesString) {
	// Check whether the moves string is bracket-enclosed
	if (movesString[0] != BRACKET_OPEN || movesString[movesString.length() - 1] != BRACKET_CLOSE) {
		throw ResourceLoader::FileFormatException("Expected bracket-enclosed moves list");
	}

	// Generate the move set
	std::map<int, const PieceMove*>* moves = new std::map<int, const PieceMove*>();

	unsigned int nestLevel = 0;
	unsigned int beginIndex = 1;
	unsigned int index = 1;

	while (index < movesString.length() - 1) {
        const char curChar = movesString[index];

        if (curChar == BRACKET_OPEN) {
			nestLevel++;
        } else if (curChar == BRACKET_CLOSE) {
        	if (nestLevel == 1) {
				const PieceMove* move = getMoveFromString(movesString.substr(beginIndex, index - beginIndex + 1));

				// Add a move to the move set
				moves->insert(std::make_pair(move->index, move));

				beginIndex = index + 2;
        	}

			nestLevel--;
        } else if (nestLevel == 0) {
        	// Check if the file is formatted correctly
        	if (movesString[index] != SEPARATOR) {
				throw ResourceLoader::FileFormatException("Expected token '" + std::to_string(SEPARATOR) + "'");
			}
        }

        index++;
	}

	return moves;
}

/**
 * Create a move from a string
 *
 * @param the string from which to generate the move
 *
 * @return a move generated from the string
 */
const PieceMove* PieceDefLoader::getMoveFromString(const std::string& moveString) {
	// Check whether the move string is bracket-enclosed
	if (moveString[0] != BRACKET_OPEN || moveString[moveString.length() - 1] != BRACKET_CLOSE) {
		throw ResourceLoader::FileFormatException("Expected bracket-enclosed move string");
	}

	// Check whether the move string has the correct number of arguments
	int numMoveArgs = getNumArgs(moveString.substr(1, moveString.length() - 2));
	if (numMoveArgs != NUM_MOVE_ARGS) {
		throw ResourceLoader::FileFormatException(
			"Expecting " + std::to_string(NUM_MOVE_ARGS) + " arguments, received " + std::to_string(numMoveArgs)
		);
	}

	unsigned int index = 1;
	unsigned int beginIndex = 1;

	// Get the index for the move
	while (index < moveString.length() - 1) {
		if (moveString[index++] == SEPARATOR) {
			break;
		}
	}

	const int moveIndex = std::stoi(moveString.substr(beginIndex, index - beginIndex));

	beginIndex = index;

    // Get the vector for the move
	while (index < moveString.length() - 1) {
		if (moveString[index++] == BRACKET_CLOSE) {
			break;
		}
	}

	const sf::Vector2i baseVector = getVectorFromString(moveString.substr(beginIndex, index - beginIndex));

    // Get the move properties
    const bool attacksFriendlies = moveString[++index] - '0';
    const bool attacksEnemies    = moveString[++index] - '0';
    const bool movesEmpty        = moveString[++index] - '0';
    const bool canLeap           = moveString[++index] - '0';
    const bool endsTurn          = moveString[++index] - '0';
    const bool isXSymmetric      = moveString[++index] - '0';
    const bool isYSymmetric      = moveString[++index] - '0';
    const bool isXYSymmetric     = moveString[++index] - '0';

	return new PieceMove(
		nullptr, moveIndex, baseVector, attacksFriendlies, attacksEnemies, movesEmpty, canLeap, endsTurn,
		isXSymmetric, isYSymmetric, isXYSymmetric
	);
}

/**
 * Determine the number of arguments in the string
 */
const int PieceDefLoader::getNumArgs(const std::string& s) {
	// Check whether the string is empty
	if (s.empty()) {
		return 0;
	}

	// Count the number of arguments
	int nestLevel = 0;
	int argCount = 1;

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
	// Check whether the string is bracket-enclosed
	if (s[0] != BRACKET_OPEN || s[s.length() - 1] != BRACKET_CLOSE) {
		throw ResourceLoader::FileFormatException("Expected bracket-enclosed string");
	}

	unsigned int beginIndex = 1;
	unsigned int index = s.find(SEPARATOR, 0);

	// Check whether a separator exists
	if (index == std::string::npos) {
		throw ResourceLoader::FileFormatException("Expected token '" + std::to_string(SEPARATOR) + "'");
	}

	const int x = std::stoi(s.substr(beginIndex, index - beginIndex));
	const int y = std::stoi(s.substr(index + 1, s.length() - index - 1));

	return sf::Vector2i(x, y);
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

	std::ifstream file(fileName, std::ios::in);
	std::string line;

	// Check whether the file is open
	if (!file.is_open()) {
		throw ResourceLoader::IOException("Unable to open file: " + fileName);
	}

	// Read each line of the file
	std::vector<std::string> pieceStrings;
	std::string pieceSoFar = "";
	int nestLevel = 0;

	// Process the file into one string per piece definition
	while (std::getline(file, line)) {
		// Check if the line is a comment
		if (line[0] == '#') continue;

		int lineBegin = 0;
        for (unsigned int i = 0; i < line.length(); i++) {
			const char curChar = line[i];
			if (curChar == BRACKET_OPEN) {
				nestLevel++;
			} else if (curChar == BRACKET_CLOSE){
				// Check if a bracket was expected
				if (nestLevel == 0) {
					throw ResourceLoader::FileFormatException(
						"Unexpected token '" + std::to_string(BRACKET_CLOSE) + "'"
					);
				} else if (nestLevel == 1) {
                    pieceSoFar += line.substr(lineBegin, i - lineBegin + 1);
                    lineBegin = i + 1;
                    pieceStrings.push_back(pieceSoFar);
                    pieceSoFar = "";
				}

				nestLevel--;
			} else if (curChar == ' ' || curChar == '\t') {
				pieceSoFar += line.substr(lineBegin, i - lineBegin);
				lineBegin = i + 1;
			}
        }

        // Add the line to the piece
        pieceSoFar += line.substr(lineBegin);
	}

	// Create the pieces from each of the piece strings
	std::map<std::string, const PieceDef*>* pieceDefs = new std::map<std::string, const PieceDef*>();
	for (std::string s : pieceStrings) {
		const PieceDef* pieceDef = pieceDefFromString(s);

		// Check if the piece definition already exists
		if (pieceDefs->find(pieceDef->name) != pieceDefs->end()) {
			throw ResourceLoader::FileFormatException("Duplicate definition of " + pieceDef->name);
		}

        pieceDefs->insert(std::make_pair(pieceDef->name, pieceDef));
	}

	// Close the file
	file.close();

	return nullptr;
}
