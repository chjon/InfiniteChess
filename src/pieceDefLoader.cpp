#include "pieceDefLoader.h"

#include <fstream>
#include "event.h"
#include "moveDef.h"
#include "numRule.h"
#include "pieceDef.h"
#include "stringUtils.h"
#include "resourceLoader.h"
#include "targetingRule.h"
#include "vectorUtils.h"



// Private object generation methods

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
const PieceDef* PieceDefLoader::getPieceDefFromString(const std::string& pieceString) {
	// Validate input
	ResourceLoader::checkBracketEnclosed(pieceString);
    ResourceLoader::checkNumArgs(pieceString.substr(1, pieceString.length() - 2), 3);

	const std::vector<std::string>* properties = StringUtils::getList(
		pieceString.substr(1, pieceString.length() - 2),
		ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
	);

	// Get the name
	unsigned int propertyIndex = 0;
	const std::string pieceName = (*properties)[propertyIndex++];

	// Get piece properties
	unsigned int index = 0;
	const bool isCheckVulnerable = (*properties)[propertyIndex][index++] - '0';
	const bool isRoyal = (*properties)[propertyIndex][index++] - '0';
	propertyIndex++;

	// Get move set
	const std::map<int, const MoveDef*>* moveSet = ResourceLoader::getMapFromString(
		(*properties)[propertyIndex++],
        (int(*)(const MoveDef*)) [](auto val){ return val->index; },
        (const MoveDef*(*)(const std::string&)) [](auto s){ return getMoveFromString(s); }
	);

	// Clean up and return
	delete properties;
    return new PieceDef(pieceName, isCheckVulnerable, isRoyal, moveSet);
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
	ResourceLoader::checkBracketEnclosed(moveString);
	ResourceLoader::checkNumArgs(moveString.substr(1, moveString.length() - 2), NUM_MOVE_ARGS);

	// Get all arguments
	const std::vector<std::string>* args = StringUtils::getList(
		moveString.substr(1, moveString.length() - 2),
		ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
	);
	unsigned int argIndex = 0;

	const int moveIndex = std::stoi((*args)[argIndex++]);
	const sf::Vector2i baseVector = VectorUtils::fromString((*args)[argIndex++]);

    // Get the move properties
    unsigned int index = 0;
    const bool canLeap       = (*args)[argIndex][index++] - '0';
    const bool endsTurn      = (*args)[argIndex][index++] - '0';
    const bool isXSymmetric  = (*args)[argIndex][index++] - '0';
    const bool isYSymmetric  = (*args)[argIndex][index++] - '0';
    const bool isXYSymmetric = (*args)[argIndex][index++] - '0';
    argIndex++;

    // Load chained moves and movement rules
	const std::vector<int>* chainedMoves = ResourceLoader::getListFromString
		((*args)[argIndex++], (int(*)(const std::string&)) [](auto s){ return std::stoi(s); });
	const std::vector<NumRule*>* scalingRules = ResourceLoader::getListFromString
		((*args)[argIndex++], (NumRule*(*)(const std::string& s)) [](auto s){ return new NumRule(s); });
	const std::vector<NumRule*>* nthStepRules = ResourceLoader::getListFromString
		((*args)[argIndex++], (NumRule*(*)(const std::string& s)) [](auto s){ return new NumRule(s); });
	const std::vector<const TargetingRule*>* targetingRules = ResourceLoader::getListFromString
		((*args)[argIndex++], getTargetingRuleFromString);

	MoveDef* newMove = new MoveDef(
		moveIndex, baseVector, canLeap, endsTurn, isXSymmetric, isYSymmetric, isXYSymmetric,
		chainedMoves, scalingRules, nthStepRules, targetingRules
	);

	// Clean up and return
	delete args;
	return newMove;
}

/**
 * Create a targeting rule from a string
 *
 * @param s the string from which to generate the targeting rule
 *
 * @return the targeting rule represented by the string
 */
const TargetingRule* PieceDefLoader::getTargetingRuleFromString(const std::string& targetingRulesString) {
	// Validate input
	ResourceLoader::checkBracketEnclosed(targetingRulesString);
	ResourceLoader::checkNumArgs(targetingRulesString.substr(1, targetingRulesString.length() - 2), NUM_TARGETTING_RULE_ARGS);

	// Get all arguments
	const std::vector<std::string>* args = StringUtils::getList(
		targetingRulesString.substr(1, targetingRulesString.length() - 2),
		ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
	);
	unsigned int argIndex = 0;

	// Get properties
    const sf::Vector2i offsetVector = VectorUtils::fromString((*args)[argIndex++]);
    const std::string targetName = (*args)[argIndex++];

    // Get data specifiers
    const std::vector<std::pair<std::string, const NumRule*>>* dataSpecifierList = ResourceLoader::getListFromString(
		(*args)[argIndex++],
        (std::pair<std::string, const NumRule*>(*)(const std::string& s)) [](auto s){
			// Validate input
			ResourceLoader::checkBracketEnclosed(s);
			ResourceLoader::checkNumArgs(s.substr(1, s.length() - 2), 2);

			// Generate pair
			const std::vector<std::string>* dataSpecArgs = StringUtils::getList(
				s.substr(1, s.length() - 2),
				ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
			);
			std::pair<std::string, const NumRule*> result =
				std::make_pair((*dataSpecArgs)[0], new NumRule((*dataSpecArgs)[1]));

			// Clean up and return
			delete dataSpecArgs;
			return result;
		}
	);

	// Create map from vector
	std::map<std::string, const NumRule*>* dataSpecifiers = new std::map<std::string, const NumRule*>();
    for (
		std::vector<std::pair<std::string, const NumRule*>>::const_iterator i = dataSpecifierList->begin();
		i != dataSpecifierList->end(); ++i
	) {
        dataSpecifiers->insert(*i);
    }

    // Get action to perform
    std::vector<Event*>* actions = ResourceLoader::getListFromString(
		(*args)[argIndex++],
		(Event*(*)(const std::string& s)) [](auto s){
			// Validate input
			ResourceLoader::checkBracketEnclosed(s);
			ResourceLoader::checkNumArgs(s.substr(1, s.length() - 2), 2);

			// Generate event
			const std::vector<std::string>* eventArgs = StringUtils::getList(
				s.substr(1, s.length() - 2),
				ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
			);
			Event* result =	new Event(nullptr, (*eventArgs)[0], (*eventArgs)[1]);

			// Clean up and return
			delete eventArgs;
			return result;
		}
	);

    // Create targeting rule
    TargetingRule* targetingRule = new TargetingRule(offsetVector, targetName, dataSpecifiers, actions);

    // Clean up and return
    delete args;
    delete dataSpecifierList;
    return targetingRule;
}



// Private helper methods

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

// Public methods

/**
 * Load piece definitions from file
 */
std::map<std::string, const PieceDef*>* PieceDefLoader::loadPieceDefs (
	const std::string& fileName, const std::string& extension
) {
	// Check whether filename is valid
	if (!ResourceLoader::isValidFileName(fileName, extension)) {
		throw ResourceLoader::FileFormatException("Invalid file name");
	}

	// Create the piece definitions
	return ResourceLoader::getMapFromString(
		removeWhiteSpace(fileName),
        (std::string(*)(const PieceDef*))[](auto val){ return val->name; },
        (const PieceDef*(*)(const std::string&))[](auto s){ return getPieceDefFromString(s); }
	);
}
