#include "renderer.h"
#include <iostream>

Renderer::Renderer(sf::RenderWindow* w) :
	window{w}
{
}

Renderer::~Renderer() {
	delete window;

	window = nullptr;
}

/**
 * Handle window resizing
 */
void Renderer::onResize() {
    sf::Vector2u windowSize = window->getSize();

    // Find the dimensions of the window in tiles, padded on each edge
    dimensionsInTiles.x = 2 * (TILE_PADDING) + 2 * std::floor(windowSize.x / tileSize / 2.f);
    dimensionsInTiles.y = 2 * (TILE_PADDING) + 2 * std::floor(windowSize.y / tileSize / 2.f);

    parity = ((dimensionsInTiles.x + dimensionsInTiles.y) / 2) % 2;

    // Find the start position for drawing tiles
    tileStartPos.x = (windowSize.x / 2.f) - tileSize * (dimensionsInTiles.x / 2.f);
    tileStartPos.y = (windowSize.y / 2.f) - tileSize * (dimensionsInTiles.y / 2.f);

    // Shift the tile start position to centre on the camera
    onCameraMove();
}

/**
 * Set the camera position
 */
void Renderer::setCameraPos(sf::Vector2f pos) {
    cameraPos = pos;

    onCameraMove();
}

/**
 * Move the camera
 */
void Renderer::moveCamera(sf::Vector2f translationVec) {
    setCameraPos(cameraPos + translationVec);
}

/**
 * Handle camera movement
 */
void Renderer::onCameraMove() {
    cameraShift.x = - tileSize * std::fmod(cameraPos.x, 2.f);
    cameraShift.y = - tileSize * std::fmod(cameraPos.y, 2.f);

    draw();
}

/**
 * Zoom the camera
 */
 void Renderer::onZoom(float delta) {
    tileSize += delta;

    if (delta > 0) {
        tileSize = std::min(tileSize, MAX_TILE_SIZE);
    } else {
        tileSize = std::max(tileSize, MIN_TILE_SIZE);
    }

    onResize();
 }

/**
 * Handle initialization
 */
void Renderer::onStartup() {
	onResize();
}

/**
 * Draw the board and pieces
 */
void Renderer::draw() {
	window->clear(sf::Color::White);

	// Draw the board
	for (unsigned int x = 0; x < dimensionsInTiles.x; x++) {
		for (unsigned int y = 0; y < dimensionsInTiles.y; y++) {
			if ((x + y) % 2 == parity) continue;

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
