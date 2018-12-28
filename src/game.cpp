#include "game.h"

Game::Game() {
	const unsigned int WIDTH  = sf::VideoMode::getDesktopMode().width;
	const unsigned int HEIGHT = sf::VideoMode::getDesktopMode().height;

	window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Infinite Chess", sf::Style::Fullscreen);
	window->setFramerateLimit(60);

	renderer = new Renderer(window);
}

Game::~Game() {
	delete renderer;

	window   = nullptr;
	renderer = nullptr;
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

	// Run the main game loop
	while (window->isOpen()) {
		if (timer.getElapsedTime() < delay) continue;
		timer.restart();

		tick();
	}
}

/**
 * Check for events and input
 */
void Game::tick() {
	// Only process events if the window is focused
	if (!window->hasFocus()) return;

	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {

		// Check whether the window needs to be closed
		case sf::Event::Closed:
			window->close();
			break;

		// Check whether the dimensions need to be updated
		case sf::Event::Resized:
			renderer->onResize();
			break;

		// Check whether the tile size needs to be updated
		case sf::Event::MouseWheelScrolled:
			renderer->onZoom(event.mouseWheelScroll.delta);
			break;

		// Check whether the window just gained focus
		case sf::Event::GainedFocus:
			renderer->draw();
			break;

		// Check whether a key was pressed
		case sf::Event::KeyPressed:
			onKeyPress(event.key);
			break;

		// Check whether the mouse was moved
		case sf::Event::MouseMoved:
			renderer->needsRedraw = true;
			break;

		default:
			break;
		}
	}

	checkKeyboard();

	renderer->draw();
}

/**
 * Handle keyboard input
 */
void Game::onKeyPress(sf::Event::KeyEvent keyEvent) {
	switch (keyEvent.code) {

	// Toggle the debug display
	case sf::Keyboard::Key::F3:
		renderer->toggleDisplayDebugData();
		break;

	// Close the window
	case sf::Keyboard::Key::Escape:
		window->close();
		break;

	default:
		break;
	}
}

/**
 * Handle continuous keyboard input
 */
void Game::checkKeyboard() {
	const float MOVE_AMOUNT = 0.2f;
	sf::Vector2f translationVec;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
		translationVec.y -= MOVE_AMOUNT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
		translationVec.y += MOVE_AMOUNT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
		translationVec.x -= MOVE_AMOUNT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		translationVec.x += MOVE_AMOUNT;
	}

	// Only move the camera if there is a non-zero movement
	if (translationVec.x != 0 || translationVec.y != 0) {
		renderer->moveCamera(translationVec);
	}
}
