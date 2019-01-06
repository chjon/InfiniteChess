#include <string>
#include "renderer.h"
#include "vectorUtils.h"
#include "moveMarker.h"

// Private utility methods

/**
 * Get the position of a tile on screen
 */
sf::Vector2i Renderer::getScreenPos(sf::Vector2i pos) const {
    return sf::Vector2i(
		tileStartPos.x + tileSize * (pos.x - cameraPos.x + dimensionsInTiles.x / 2),
		tileStartPos.y + tileSize * (pos.y - cameraPos.y + dimensionsInTiles.y / 2)
	);
}

/**
 * Draw the board
 */
void Renderer::drawBoard() const {
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
}

/**
 * Draw the pieces
 */
void Renderer::drawPieces() const {
	std::map<sf::Vector2i, GamePiece*, VectorUtils::cmpVectorLexicographically>* pieces = &(game->pieceTracker->pieces);

	// Draw the pieces
    for (std::map<sf::Vector2i, GamePiece*, VectorUtils::cmpVectorLexicographically>::iterator it = pieces->begin(); it != pieces->end(); it++) {
        drawPiece(it->second);
    }
}

/**
 * Draw tile overlays
 */
void Renderer::drawOverlays() const {
	const sf::Vector2i mousePos = getMouseTilePosition();
	GamePiece* selectedPiece = game->controller->getSelectedPiece();

    // Draw selection overlay
    if (selectedPiece != nullptr) {
		drawTile(selectedPiece->pos.x, selectedPiece->pos.y, PIECE_SELECTED_COLOR);

		// Draw possible moves
		for (std::map<sf::Vector2i, MoveMarker*>::iterator it = selectedPiece->moveMarkers.begin();
			it != selectedPiece->moveMarkers.end();
			++it
		) {
			if (!it->second->canMove() && !displayDebugData) continue;
			drawTile(it->first.x, it->first.y, PIECE_SELECTED_COLOR);
		}
    }

    // Draw mouse overlay

	// If no piece is selected, only pieces should be selectable
    if (selectedPiece == nullptr) {
		if (game->pieceTracker->getPiece(mousePos) == nullptr) {
			drawTile(mousePos.x, mousePos.y, MOUSE_INVALID_COLOR);
		} else {
			drawTile(mousePos.x, mousePos.y, MOUSE_VALID_COLOR);
		}

	// If a piece is selected, only valid move positions should be selectable
    } else {
		if (selectedPiece->canMove(mousePos)) {
			drawTile(mousePos.x, mousePos.y, MOUSE_VALID_COLOR);
		} else {
			drawTile(mousePos.x, mousePos.y, MOUSE_INVALID_COLOR);
		}
    }

    // Check if debug data should be drawn
    if (displayDebugData) {
		drawTile(mousePos.x, mousePos.y, MOUSE_DEBUG_COLOR);
    }
}

/**
 * Draw debug text
 *
 * @param s   the text to draw
 * @param row the level at which to draw the text
 */
void Renderer::drawDebugText(const std::string& s, const unsigned int row) const {
	const unsigned int FONT_SIZE = 20;
	sf::Text label(s, debugFont, FONT_SIZE);
	label.setFillColor(sf::Color::White);
	label.setPosition(0, row * FONT_SIZE);
	window->draw(label);
}

/**
 * Draw a game piece
 *
 * @param x the x position to draw to
 * @param y the y position to draw to
 * @param c the color to fill with
 */
void Renderer::drawPiece(GamePiece* p) const {
    sf::CircleShape s(tileSize / 2);
    s.setFillColor(p->team);

	s.setPosition(
		tileStartPos.x + tileSize * (p->pos.x - cameraPos.x + dimensionsInTiles.x / 2),
		tileStartPos.y + tileSize * (p->pos.y - cameraPos.y + dimensionsInTiles.y / 2)
	);

	window->draw(s);
}

/**
 * Draw the tile at (x,y)
 *
 * @param x the x position to draw to
 * @param y the y position to draw to
 * @param c the color to fill with
 */
void Renderer::drawTile(const int x, const int y, const sf::Color c) const {
    sf::RectangleShape s(sf::Vector2f(tileSize, tileSize));
    s.setFillColor(c);
	s.setPosition(
		tileStartPos.x + tileSize * (x - cameraPos.x + dimensionsInTiles.x / 2),
		tileStartPos.y + tileSize * (y - cameraPos.y + dimensionsInTiles.y / 2)
	);

	window->draw(s);
}

/**
 * Draw debug data
 */
void Renderer::drawDebug() const {
	sf::Vector2f mousePos = getMousePosition();

	std::string s;
	int row = 0;

	s = "Camera position: (" + std::to_string(cameraPos.x) + ", " + std::to_string(cameraPos.y) + ")";
	drawDebugText(s, row++);
	s = "Mouse position: (" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")";
	drawDebugText(s, row++);
	s = "Tile size: " + std::to_string(tileSize);
	drawDebugText(s, row++);

	GamePiece* selectedPiece = game->controller->getSelectedPiece();
	if (selectedPiece == nullptr) {
		s = "Selected piece: null";
		drawDebugText(s, row++);
	} else {
		s = "Selected piece: " + selectedPiece->name;
		drawDebugText(s, row++);
		s = "Selected piece dir: " + std::to_string(selectedPiece->dir);
		drawDebugText(s, row++);
		s = "Selected piece pos: (" + std::to_string(selectedPiece->pos.x) + ", " + std::to_string(selectedPiece->pos.y) + ")";
		drawDebugText(s, row++);
		s = "Selected piece team: rgba(" +
			std::to_string(selectedPiece->team.r) + "," +
			std::to_string(selectedPiece->team.g) + "," +
			std::to_string(selectedPiece->team.b) + "," +
			std::to_string(selectedPiece->team.a) + ")";
		drawDebugText(s, row++);
	}
}



// Public constructors

/**
 * Constructor
 *
 * @param w The window for the renderer
 */
Renderer::Renderer(Game* g, sf::RenderWindow* w) :
	game{g},
	window{w}
{
	window->setFramerateLimit(MAX_FRAMERATE);
}

/**
 * Destructor
 */
Renderer::~Renderer() {
	game   = nullptr;
	window = nullptr;
}



// Public event handlers

/**
 * Handle initialization
 */
void Renderer::onStartup() {
	// Load resources
	debugFont.loadFromFile(FONT_DIRECTORY + DEBUG_FONT_FILENAME);

	onResize(window->getSize().x, window->getSize().y);
}

/**
 * Handle window resizing
 */
void Renderer::onResize(const unsigned int width, const unsigned int height) {
    window->setView(sf::View(sf::FloatRect(0, 0, width, height)));

	// Find the dimensions of the window in tiles, padded on each edge
	dimensionsInTiles.x = 2 * (TILE_PADDING + std::floor(width / tileSize / 2.f));
	dimensionsInTiles.y = 2 * (TILE_PADDING + std::floor(height / tileSize / 2.f));

	parity = ((dimensionsInTiles.x + dimensionsInTiles.y) / 2) % 2;

	// Find the start position for drawing tiles
	tileStartPos.x = (width / 2.f) - tileSize * (dimensionsInTiles.x / 2.f);
	tileStartPos.y = (height / 2.f) - tileSize * (dimensionsInTiles.y / 2.f);

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

	onResize(window->getSize().x, window->getSize().y);
}



// Public accessors

/**
 * Get the cursor's exact tile coordinates
 */
sf::Vector2f Renderer::getMousePosition() const {
	sf::Vector2i screenPos = sf::Mouse::getPosition(*window);

	return sf::Vector2f(
		(screenPos.x - tileStartPos.x) / tileSize + cameraPos.x - dimensionsInTiles.x / 2,
		(screenPos.y - tileStartPos.y) / tileSize + cameraPos.y - dimensionsInTiles.y / 2
	);
}

/**
 * Get the cursor's tile coordinates
 */
sf::Vector2i Renderer::getMouseTilePosition() const {
	sf::Vector2i screenPos = sf::Mouse::getPosition(*window);

	return sf::Vector2i(
		std::floor((screenPos.x - tileStartPos.x) / tileSize + cameraPos.x - dimensionsInTiles.x / 2),
		std::floor((screenPos.y - tileStartPos.y) / tileSize + cameraPos.y - dimensionsInTiles.y / 2)
	);
}

/**
 * Get the tile dimensions
 */
sf::Vector2u Renderer::getTileDimensions() const {
	return dimensionsInTiles;
}

/**
 * Determine whether a certain tile position is within the bounds of the screen
 */
bool Renderer::isRenderable(sf::Vector2i pos) const {
    sf::Vector2i screenPos = getScreenPos(pos);

    bool renderable = !((screenPos.x) < -tileSize ||
		(screenPos.x > 0 && (unsigned int) screenPos.x > window->getSize().x) ||
		(screenPos.y) < -tileSize ||
		(screenPos.y > 0 && (unsigned int) screenPos.y > window->getSize().y)
	);

    return renderable;
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
	drawBoard();
	drawPieces();
	drawOverlays();

	// Draw debug data
	if (displayDebugData) {
		drawDebug();
	}

	window->display();
	needsRedraw = false;
}
