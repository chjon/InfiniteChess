#ifndef CHESS_BOARD_LOADER_H
#define CHESS_BOARD_LOADER_H

#include <SFML/Graphics.hpp>
#include "piece.h"
#include "stringUtils.h"
#include "vectorUtils.h"

// Class declaration
class BoardLoader {
private:
	// Constants
	static const unsigned int COLOR_NUM_ARGS = 4;
	static const unsigned int BOARD_NUM_ARGS = 3;
	static const unsigned int PIECE_NUM_ARGS = 6;

    // Object generation methods

    /**
     * Create a color from a string
     */
    inline static sf::Color getColorFromString(const std::string& colorString) {
		// Validate input
		ResourceLoader::checkBracketEnclosed(colorString);
        ResourceLoader::checkNumArgs(colorString.substr(1, colorString.length() - 2), COLOR_NUM_ARGS);

		unsigned int argIndex = 0;
		std::vector<std::string>* args = StringUtils::getList(
			colorString.substr(1, colorString.length() - 2),
			ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
		);

		const unsigned int R = std::stoi((*args)[argIndex++]);
		const unsigned int G = std::stoi((*args)[argIndex++]);
		const unsigned int B = std::stoi((*args)[argIndex++]);
		const unsigned int A = std::stoi((*args)[argIndex++]);

		// Clean up and return
		delete args;
		return sf::Color(R, G, B, A);
    }

    /**
     * Create a team from a string
     */
    inline static const std::pair<const unsigned int, std::pair<const std::string, sf::Color>> getTeamFromString (
		const std::string& teamString
	) {
		// Validate input
		ResourceLoader::checkBracketEnclosed(teamString);

		unsigned int argIndex = 0;
		std::vector<std::string>* args = StringUtils::getList(
			teamString.substr(1, teamString.length() - 2),
			ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
		);

		const unsigned int index = std::stoi((*args)[argIndex++]);
        const std::string teamName = (*args)[argIndex++];
        sf::Color teamColor = getColorFromString((*args)[argIndex++]);

        return std::make_pair(index, std::make_pair(teamName, teamColor));
	}

	/**
	 * Create a piece from a string
	 */
	inline static Piece* getPieceFromString(
		const std::string& pieceString,
		const std::map<std::string, const PieceDef*>* pieceDefs,
		const std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams
	) {
		// Validate input
		ResourceLoader::checkBracketEnclosed(pieceString);
		ResourceLoader::checkNumArgs(pieceString.substr(1, pieceString.length() - 2), PIECE_NUM_ARGS);

		unsigned int argIndex = 0;
		std::vector<std::string>* args = StringUtils::getList(
			pieceString.substr(1, pieceString.length() - 2),
			ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
		);

		// Check whether a piece definition with the desired name exists
        const std::string pieceName = (*args)[argIndex++];
		std::map<std::string, const PieceDef*>::const_iterator defIter = pieceDefs->find(pieceName);
		if (defIter == pieceDefs->end()) {
			throw ResourceLoader::FileFormatException("Could not find definition for " + pieceName);
		}

		// Check whether a team with the desired index exists
		const unsigned int teamIndex = std::stoi((*args)[argIndex++]);
		std::map<const unsigned int, std::pair<const std::string, sf::Color>>::const_iterator teamIter =
			teams->find(teamIndex);
		if (teamIter == teams->end()) {
			throw ResourceLoader::FileFormatException(
				"Could not find a team with the index " + std::to_string(teamIndex)
			);
		}

		// Get other piece properties
        sf::Vector2i pos = VectorUtils::fromString((*args)[argIndex++]);
        const unsigned int dir = std::stoi((*args)[argIndex++]) % 4;
        const unsigned int moveCount = std::stoi((*args)[argIndex++]);
        const int lastMove = std::stoi((*args)[argIndex++]);

		// Clean up and return
		delete args;
		return new Piece(
			defIter->second,
			teamIter->first,
			pos,
			(PieceDef::Direction) dir,
			moveCount,
			lastMove
		);
	}

    /**
     * Create the teams from a string
     */
	inline static std::map<const unsigned int, std::pair<const std::string, sf::Color>>* getTeamsFromString (
		const std::string& teamsString
	) {
		// Validate input
		ResourceLoader::checkBracketEnclosed(teamsString);

		std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams =
			new std::map<const unsigned int, std::pair<const std::string, sf::Color>>();
		std::vector<std::string>* teamStrings = StringUtils::getList(
			teamsString.substr(1, teamsString.length() - 2),
			ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
		);

        for (std::vector<std::string>::iterator i = teamStrings->begin(); i != teamStrings->end(); ++i) {
            std::pair<const unsigned int, std::pair<const std::string, sf::Color>> team =
				getTeamFromString(*i);

            // Check whether a team already exists with the desired index
			if (teams->find(team.first) != teams->end()) {
				throw ResourceLoader::FileFormatException(
					"Duplicate team index for " + std::to_string(team.first)
				);
			}

			// Add the team
			teams->insert(team);
        }

        // Clean up and return
        delete teamStrings;
        return teams;
	}

    /**
     * Create the pieces from a string
     */
    inline static std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>* getPiecesFromString (
		const std::string& piecesString, const std::map<std::string, const PieceDef*>* pieceDefs,
		const std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams
	) {
		// Validate input
		ResourceLoader::checkBracketEnclosed(piecesString);

		std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>* pieces =
			new std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>();
		std::vector<std::string>* pieceStrings = StringUtils::getList(
			piecesString.substr(1, piecesString.length() - 2),
			ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
		);

        // Create the pieces
        for (std::vector<std::string>::iterator i = pieceStrings->begin(); i != pieceStrings->end(); ++i) {
            Piece* piece = getPieceFromString(*i, pieceDefs, teams);

            // Check whether a piece already exists at the desired location
			if (pieces->find(piece->getPos()) != pieces->end()) {
				throw ResourceLoader::FileFormatException(
					"Duplicate piece definition for " + VectorUtils::toString(piece->getPos())
				);
			}

            // Add the piece
            pieces->insert(std::make_pair(piece->getPos(), piece));
        }

        // Clean up and return
        delete pieceStrings;
        return pieces;
	}

    /**
	 * Load game board from file
	 */
	inline static std::tuple<
		std::map<const unsigned int, std::pair<const std::string, sf::Color>>*,
        unsigned int,
		std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>*
	> getBoardFromString(const std::string& boardString, std::map<std::string, const PieceDef*>* pieceDefs) {
        // Validate input
		ResourceLoader::checkBracketEnclosed(boardString);
		ResourceLoader::checkNumArgs(boardString.substr(1, boardString.length() - 2), BOARD_NUM_ARGS);

		const std::vector<std::string>* properties = StringUtils::getList(
			boardString.substr(1, boardString.length() - 2),
			ResourceLoader::SEPARATOR, ResourceLoader::BRACKET_OPEN, ResourceLoader::BRACKET_CLOSE
		);

		// Get the teams
		unsigned int propertyIndex = 0;
		std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams =
			getTeamsFromString((*properties)[propertyIndex++]);

        // Get the current turn
        const unsigned int curTurn = std::stoi((*properties)[propertyIndex++]);

        // Get the pieces
        std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>* pieces =
			getPiecesFromString((*properties)[propertyIndex++], pieceDefs, teams);

		return std::make_tuple(teams, curTurn, pieces);
	}

public:

	/**
	 * Load game board from file
	 */
	inline static std::tuple<
		std::map<const unsigned int, std::pair<const std::string, sf::Color>>*,
        unsigned int,
		std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>*
	> loadBoard(const std::string& fileName, std::map<std::string, const PieceDef*>* pieceDefs) {
		// Check whether filename is valid
		if (!ResourceLoader::isValidFileName(fileName, ".chess")) {
			throw ResourceLoader::FileFormatException("Invalid file name");
		}

		return getBoardFromString(ResourceLoader::removeWhiteSpace(fileName), pieceDefs);
	}
};

#endif // CHESS_BOARD_LOADER_H
