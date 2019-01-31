#include "game.h"

#include <iostream>
#include "boardLoader.h"
#include "pieceDefLoader.h"
#include "resourceLoader.h"
#include "stringUtils.h"

// Public constructors

/**
 * Constructor
 */
Game::Game() {
	const unsigned int WIDTH  = sf::VideoMode::getDesktopMode().width * 3. / 4.;
	const unsigned int HEIGHT = sf::VideoMode::getDesktopMode().height * 3. / 4.;

	window = new sf::RenderWindow(
		sf::VideoMode(WIDTH, HEIGHT),
		"Infinite Chess",
		sf::Style::Default
	);

	renderer       = new Renderer(this, window);
	inputHandler   = new InputHandler(this, window, renderer);
	pieceTracker   = new PieceTracker(this);
	controller     = new Controller(this, pieceTracker);
}

/**
 * Destructor
 */
Game::~Game() {
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

	delete controller;
    delete pieceTracker;
	delete inputHandler;
	delete renderer;
	delete window;

	controller     = nullptr;
	pieceTracker   = nullptr;
	inputHandler   = nullptr;
	renderer       = nullptr;
	window         = nullptr;
}



// Public methods

/**
 * Run the game
 */
void Game::run() {
	// Try loading resources
	try {
		pieceDefs = PieceDefLoader::loadPieceDefs("res/pieces.def");
		textures  = ResourceLoader::loadTextures(ResourceLoader::getListFromMap(pieceDefs,
			(std::string(*)(const std::string, const PieceDef*)) [](auto name, auto pieceDef){
				return name;
			}
		), "res/textures/", ".png");

		board = BoardLoader::loadBoard("res/initial.chess", pieceDefs);
	} catch (ResourceLoader::FileFormatException ex) {
        std::cout << "FileFormatException: " << ex.what() << std::endl;
        return;
	} catch (ResourceLoader::IOException ex) {
        std::cout << "IOException: " << ex.what() << std::endl;
        return;
	} catch (StringUtils::ParseException ex) {
        std::cout << "ParseException: " << ex.what() << std::endl;
        return;
	}

	// Initialize everything
	pieceTracker->onStartup(std::get<2>(board));
	renderer->onStartup();
	controller->onStartup();

	sf::Clock timer;
	sf::Time delay = sf::seconds(0.01f);

	// Run the main game loop
	while (window->isOpen()) {
		// Wait until the tick has passed
		if (timer.getElapsedTime() < delay) continue;

		// Schedule the next tick
		timer.restart();

		// Perform actions for the tick
		inputHandler->tick();
		renderer->draw();
	}
}
