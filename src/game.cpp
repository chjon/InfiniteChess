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
		std::map<std::string, const PieceDef*>* pieceDefs = PieceDefLoader::loadPieceDefs("res/pieces.def");
		std::map<std::string, sf::Texture*>* textures = ResourceLoader::loadTextures(
			ResourceLoader::getListFromMap(pieceDefs,
				(std::string(*)(const std::string, const PieceDef*)) [](auto name, auto pieceDef){
					return name;
				}
			), "res/textures/", ".png"
		);

		std::tuple<
			std::map<const unsigned int, std::pair<const std::string, sf::Color>>*,
			unsigned int,
			std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>*
		> board = BoardLoader::loadBoard("saves/initial.chess", pieceDefs);

		// Initialize everything
		pieceTracker->onStartup(pieceDefs, std::get<2>(board));
		renderer->onStartup(textures, std::get<0>(board));
		controller->onStartup(std::get<0>(board), std::get<1>(board));

		// Clean up
		pieceDefs = nullptr;
		textures = nullptr;

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

// Event processors

void Game::onCameraChange() {
	pieceTracker->onCameraChange();
}

void Game::onGeneration(MoveMarker* marker) {
    controller->onGeneration(marker);
}
