#ifndef CHESS_RESOURCE_LOADER_H
#define CHESS_RESOURCE_LOADER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <exception>

// Forward declarations
class PieceDef;
class PieceTracker;
class Renderer;
class PieceDef;



// Class declaration
class ResourceLoader {
private:
	// Constants
    const std::string PIECE_DEFS_FILE = "res/pieces.def";
    const std::string TEXTURES_DIRECTORY = "res/textures/";
    const std::string TEXTURES_EXTENSION = ".png";

	// Members
	PieceTracker* pieceTracker;
	std::map<std::string, const PieceDef*>* pieceDefs;
	std::map<std::string, sf::Texture*>* textures;

	// Utility methods
    void loadPieceDefs(std::string fileName);
    void loadTextures();

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

	// Object generation
	template <typename T> static std::vector<T>* getListFromString(
		const std::string& listString,
		T(*objectFromString)(const std::string& s)
	);
	template <typename K, typename V> static std::map<K, V>* getMapFromString(
		const std::string& listString,
		K(*keyFromObject)(V),
		V(*objectFromString)(const std::string&)
	);

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
