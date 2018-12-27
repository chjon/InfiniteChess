#include "game.h"

Game::Game() {
	const unsigned int WIDTH  = sf::VideoMode::getDesktopMode().width;
    const unsigned int HEIGHT = sf::VideoMode::getDesktopMode().height;

	renderer = new Renderer(WIDTH, HEIGHT, "Infinite Chess", sf::Style::None);
}

Game::~Game() {
	delete renderer;
}

/**
 * Run the game
 */
void Game::run() {
	// Initialize everything
	renderer->onStartup();
	sf::Vector2f pos;

	sf::Clock timer;
	sf::Time delay = sf::seconds(0.01f);

	// Run
	while (renderer->windowIsOpen()) {
        if (timer.getElapsedTime() < delay) continue;
        timer.restart();

		renderer->tick();
	}
}
