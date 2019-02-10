#ifndef CHESS_PIECE_DEF_LOADER_H
#define CHESS_PIECE_DEF_LOADER_H

#include <SFML/Graphics.hpp>
#include "../components/event.h"
#include "../components/moveDef.h"
#include "../components/numRule.h"
#include "../components/targetingRule.h"
#include "../components/pieceDef.h"
#include "resourceLoader.h"
#include "../utils/vectorUtils.h"

// Class declaration
class PieceDefLoader {
private:
    // Constants
	static const int NUM_MOVE_ARGS = 8;
	static const int NUM_TARGETTING_RULE_ARGS = 4;

	// Object generation methods

    /**
	 * Create a targeting rule from a string
	 *
	 * @param s the string from which to generate the targeting rule
	 *
	 * @return the targeting rule represented by the string
	 */
	inline static const TargetingRule* getTargetingRuleFromString(const std::string& targetingRulesString) {
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

    /**
	 * Create a move from a string
	 *
	 * @param the string from which to generate the move
	 *
	 * @return a move generated from the string
	 */
	inline static const MoveDef* getMoveFromString(const std::string& moveString) {
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
		const bool endsTurn      = (*args)[argIndex][index++] - '0';
		const bool isXSymmetric  = (*args)[argIndex][index++] - '0';
		const bool isYSymmetric  = (*args)[argIndex][index++] - '0';
		const bool isXYSymmetric = (*args)[argIndex][index++] - '0';
		argIndex++;

		// Load chained moves and movement rules
		const std::vector<int>* chainedMoves = ResourceLoader::getListFromString
			((*args)[argIndex++], (int(*)(const std::string&)) [](auto s){ return std::stoi(s); });
		const std::vector<NumRule*>* leapingRules = ResourceLoader::getListFromString
			((*args)[argIndex++], (NumRule*(*)(const std::string& s)) [](auto s){ return new NumRule(s); });
		const std::vector<NumRule*>* scalingRules = ResourceLoader::getListFromString
			((*args)[argIndex++], (NumRule*(*)(const std::string& s)) [](auto s){ return new NumRule(s); });
		const std::vector<NumRule*>* nthStepRules = ResourceLoader::getListFromString
			((*args)[argIndex++], (NumRule*(*)(const std::string& s)) [](auto s){ return new NumRule(s); });
		const std::vector<const TargetingRule*>* targetingRules = ResourceLoader::getListFromString
			((*args)[argIndex++], getTargetingRuleFromString);

		MoveDef* newMove = new MoveDef(
			moveIndex, baseVector, endsTurn, isXSymmetric, isYSymmetric, isXYSymmetric,
			chainedMoves, leapingRules, scalingRules, nthStepRules, targetingRules
		);

		// Clean up and return
		delete args;
		return newMove;
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
	inline static const PieceDef* getPieceDefFromString(const std::string& pieceString) {
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

public:
	/**
	 * Load piece definitions from file
	 */
	inline static std::map<std::string, const PieceDef*>* loadPieceDefs (
		const std::string& fileName
	) {
		// Check whether filename is valid
		if (!ResourceLoader::isValidFileName(fileName, ".def")) {
			throw ResourceLoader::FileFormatException("Invalid file name");
		}

		// Create the piece definitions
		return ResourceLoader::getMapFromString(
			ResourceLoader::removeWhiteSpace(fileName),
			(std::string(*)(const PieceDef*))[](auto val){ return val->name; },
			(const PieceDef*(*)(const std::string&))[](auto s){ return getPieceDefFromString(s); }
		);
	}
};

#endif // CHESS_PIECE_DEF_LOADER_H
