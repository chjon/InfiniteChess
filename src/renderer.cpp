#include "renderer.h"
#include <iostream>
#include <string>

// Private utility methods

/**
 * Draw debug text
 *
 * @param s   the text to draw
 * @param row the level at which to draw the text
 */
void Renderer::drawDebugText(const std::string& s, const unsigned int row) {
	const unsigned int FONT_SIZE = 20;
	sf::Text label(s, debugFont, FONT_SIZE);
	label.setFillColor(sf::Color::White);
	label.setPosition(0, row * FONT_SIZE);
	window->draw(label);
}

/**
 * Draw the tile at (x,y)
 *
 * @param x the x position to draw to
 * @param y the y position to draw to
 * @param c the color to fill with
 */
void Renderer::draw(const int x, const int y, const sf::Color c) {
	sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
	tile.setFillColor(c);
	tile.setPosition(
		tileStartPos.x + tileSize * (x - cameraPos.x + dimensionsInTiles.x / 2),
		tileStartPos.y + tileSize * (y - cameraPos.y + dimensionsInTiles.y / 2)
	);

	window->draw(tile);
}

/**
 * Draw debug data
 */
void Renderer::drawDebug() {
	sf::Vector2f mousePos = getMousePosition();
	draw(0, 0, sf::Color::White);
	draw(std::floor(mousePos.x), std::floor(mousePos.y), sf::Color(255, 255, 0, 150));

	std::string s;
	int row = 0;

	s = "Camera position: (" + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ")";
	drawDebugText(s, row++);
	s = "Mouse position: (" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")";
	drawDebugText(s, row++);
	s = "Tile size: (" + std::to_string(tileSize) + ")";
	drawDebugText(s, row++);
}



// Public constructors

/**
 * Constructor
 *
 * @param w The window for the renderer
 */
Renderer::Renderer(sf::RenderWindow* w) :
	window{w}
{
	window->setFramerateLimit(MAX_FRAMERATE);
}

/**
 * Destructor
 */
Renderer::~Renderer() {
	window = nullptr;
}



// Public event handlers

/**
 * Handle initialization
 */
void Renderer::onStartup() {
	// Load resources
	debugFont.loadFromFile(FONT_DIRECTORY + DEBUG_FONT_FILENAME);

	onResize();
}

/**
 * Handle window resizing
 */
void Renderer::onResize() {
	sf::Vector2u windowSize = window->getSize();

	// Find the dimensions of the window in tiles, padded on each edge
	dimensionsInTiles.x = 2 * (TILE_PADDING + std::floor(windowSize.x / tileSize / 2.f));
	dimensionsInTiles.y = 2 * (TILE_PADDING + std::floor(windowSize.y / tileSize / 2.f));

	parity = ((dimensionsInTiles.x + dimensionsInTiles.y) / 2) % 2;

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
	cameraShift.x = - tileSize * std::fmod(cameraPos.x, 2.f);
	cameraShift.y = - tileSize * std::fmod(cameraPos.y, 2.f);

	needsRedraw = true;
}

/**
 * Zoom the camera
 */
void Renderer::onZoom(const float delta) {
	tileSize += delta;

	if (delta > 0) {
		tileSize = std::min(tileSize, MAX_TILE_SIZE);
	} else {
		tileSize = std::max(tileSize, MIN_TILE_SIZE);
	}

	onResize();
}



// Public accessors

/**
 * Get the cursor's tile coordinates
 */
sf::Vector2f Renderer::getMousePosition() const {
	sf::Vector2i screenPos = sf::Mouse::getPosition();

	return sf::Vector2f(
		(screenPos.x - tileStartPos.x) / tileSize + cameraPos.x - dimensionsInTiles.x / 2,
		(screenPos.y - tileStartPos.y) / tileSize + cameraPos.y - dimensionsInTiles.y / 2
	);
}



// Public mutators

/**
 * Set the camera position
 */
void Renderer::setCameraPos(const sf::Vector2f pos) {
	cameraPos = pos;
	onCameraMove();
}

/**
 * Move the camera
 */
void Renderer::moveCamera(const sf::Vector2f translationVec) {
	setCameraPos(cameraPos + translationVec);
}

/**
 * Toggle displaying debug data
 */
void Renderer::toggleDisplayDebugData() {
	displayDebugData ^= 1;
	needsRedraw = true;
}



// Public utility methods

/**
 * Draw the board and pieces
 */
void Renderer::draw() {
	// Do nothing if a redraw isn't needed
	if (!needsRedraw) {
		return;
	}

	window->clear(BACKGROUND_COLOR);

	sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
	tile.setFillColor(FOREGROUND_COLOR);

	// Draw the board
	for (unsigned int x = 0; x < dimensionsInTiles.x; x++) {
		for (unsigned int y = 0; y < dimensionsInTiles.y; y++) {
			// Determine whether the tile needs to be colored
			if ((x + y) % 2 == parity) continue;

			tile.setPosition(
				tileStartPos.x + cameraShift.x + tileSize * x,
				tileStartPos.y + cameraShift.y + tileSize * y
			);

			window->draw(tile);
		}
	}

	// Draw debug data
	if (displayDebugData) {
		drawDebug();
	}

	window->display();
	needsRedraw = false;
}
