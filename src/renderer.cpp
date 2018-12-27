#include "renderer.h"

Renderer::Renderer(sf::RenderWindow& w) :
	window{&w}
{
	window->setFramerateLimit(60);
}

Renderer::Renderer(unsigned int w, unsigned int h, const std::string& title, const int style) {
	window = new sf::RenderWindow(sf::VideoMode(w, h), title, style);
	window->setFramerateLimit(60);
}

Renderer::~Renderer() {
	delete window;
}

/**
 * Handle window resizing
 */
void Renderer::onResize() {
    sf::Vector2u windowSize = window->getSize();

    // Find the dimensions of the window in tiles, padded on each edge
    dimensionsInTiles.x = 2 * (TILE_PADDING) + 2 * std::floor(windowSize.x / tileSize / 2.f);
    dimensionsInTiles.y = 2 * (TILE_PADDING) + 2 * std::floor(windowSize.y / tileSize / 2.f);

    // Find the start position for drawing tiles
    tileStartPos.x = (windowSize.x / 2.f) - tileSize * (dimensionsInTiles.x / 2.f);
    tileStartPos.y = (windowSize.y / 2.f) - tileSize * (dimensionsInTiles.y / 2.f);

    // Shift the tile start position to centre on the camera
    onCameraMove();
}

/**
 * Handle camera movement
 */
void Renderer::onCameraMove() {
    cameraShift.x = cameraPos.x - tileSize * std::fmod(cameraPos.x, 2.f);
    cameraShift.y = cameraPos.y - tileSize * std::fmod(cameraPos.y, 2.f);

    draw();
}

/**
 * Handle initialization
 */
void Renderer::onStartup() {
	onResize();
}

/**
 * Move the camera
 */
void Renderer::setCameraPos(sf::Vector2f pos) {
    cameraPos = pos;

    onCameraMove();
}

/**
 * Check for window events
 */
void Renderer::tick() {
	sf::Event event;
	while (window->pollEvent(event)) {
		// Check whether the window needs to be closed
		if (event.type == sf::Event::Closed) {
			window->close();
		} else if (event.type == sf::Event::Resized) {

		}
	}

	draw();
}

bool Renderer::windowIsOpen() {
	return window->isOpen();
}

/**
 * Draw the board and pieces
 */
void Renderer::draw() {
	window->clear(sf::Color::White);

	// Draw the board
	for (int x = 0; x < dimensionsInTiles.x; x++) {
		for (int y = 0; y < dimensionsInTiles.y; y++) {
			if ((x + y) % 2 == 0) continue;

            sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
            tile.setFillColor(sf::Color::Green);
			tile.setPosition(
				tileStartPos.x + cameraShift.x + tileSize * x,
				tileStartPos.y + cameraShift.y + tileSize * y
			);

			window->draw(tile);
		}
	}

	window->display();
}
