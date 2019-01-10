#ifndef CHESS_RESOURCE_LOADER_H
#define CHESS_RESOURCE_LOADER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <exception>

// Forward declarations
class GamePiece;
class PieceTracker;
class Renderer;



// Class declaration
class ResourceLoader {
private:
	// Constants
    const std::string PIECE_DEFS_FILE = "res/pieces.def";
    const std::string TEXTURES_DIRECTORY = "res/textures/";
    const std::string TEXTURES_EXTENSION = ".png";

	// Members
	PieceTracker* pieceTracker;
	std::map<std::string, GamePiece*>* pieceDefs;
	std::map<std::string, sf::Texture*>* textures;

	// Utility methods
    void loadPieceDefs(std::string fileName);

public:
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

	// Constructors
	ResourceLoader(PieceTracker* p);
	~ResourceLoader();

	// Event handlers
	void onStartUp();

    // Friends
    friend PieceTracker;
    friend Renderer;
};

#endif // CHESS_RESOURCE_LOADER_H
