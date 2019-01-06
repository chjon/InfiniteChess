#include <iostream>
#include "game.h"

// Public constructors

/**
 * Constructor
 */
Game::Game() {
	const unsigned int WIDTH  = sf::VideoMode::getDesktopMode().width  * 2. / 3.;
	const unsigned int HEIGHT = sf::VideoMode::getDesktopMode().height * 2. / 3.;

	window = new sf::RenderWindow(
		sf::VideoMode(WIDTH, HEIGHT),
		"Infinite Chess",
		sf::Style::Default
	);

	renderer       = new Renderer(this, window);
	inputHandler   = new InputHandler(this, window, renderer);
	pieceTracker   = new PieceTracker(this);
	controller     = new Controller(this, pieceTracker);
	resourceLoader = new ResourceLoader(pieceTracker);
}

/**
 * Destructor
 */
Game::~Game() {
	delete resourceLoader;
	delete controller;
    delete pieceTracker;
	delete inputHandler;
	delete renderer;
	delete window;

	resourceLoader = nullptr;
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
	// Initialize everything

	// Try loading resources
	try {
		resourceLoader->onStartUp();
	} catch (ResourceLoader::FileFormatException ex) {
        std::cout << "FileFormatException: " << ex.what() << std::endl;
        return;
	} catch (ResourceLoader::IOException ex) {
        std::cout << "IOException: " << ex.what() << std::endl;
        return;
	}

	renderer->onStartup();
	pieceTracker->onStartup();

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
