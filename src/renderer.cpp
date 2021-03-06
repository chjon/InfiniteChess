#include "renderer.h"

#include <string>
#include "components/moveMarker.h"
#include "component_trackers/moveTracker.h"
#include "components/piece.h"
#include "utils/vectorUtils.h"
#include "ui/windowLayer.h"

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
			if ((x + y) % 2 == parity) {
				tile.setPosition(
					tileStartPos.x + cameraShift.x + tileSize * x,
					tileStartPos.y + cameraShift.y + tileSize * y
				);

				window->draw(tile);
			}
		}
	}
}

/**
 * Draw the layers
 */
void Renderer::drawUILayers() const {
	// Check whether the menu should be drawn
	if (!displayMenu) {
		return;
	}

	for (std::vector<WindowLayer*>::const_iterator i = layers.begin(); i != layers.end(); ++i) {
        (*i)->draw(uiTextures, debugFont, window);
	}
}

/**
 * Draw the pieces
 */
void Renderer::drawPieces() const {
	std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>* pieces = game->pieceTracker->pieces;

	// Draw the pieces
    for (std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>::iterator it = pieces->begin();
		it != pieces->end();
		++it
	) {
        drawPiece(it->second);
    }
}

/**
 * Draw tile overlays
 */
void Renderer::drawOverlays() const {
	const sf::Vector2i mousePos = getMouseTilePosition();
	Piece* selectedPiece = game->controller->getSelectedPiece();

    // Draw selection overlay
    if (selectedPiece != nullptr) {
		drawTile(selectedPiece->getPos().x, selectedPiece->getPos().y, PIECE_SELECTED_COLOR);

		// Draw possible moves
		for (std::map<const MoveDef*, std::map<
				sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically>*
			>::iterator i = selectedPiece->moveTracker->moveMarkers->begin();
			i != selectedPiece->moveTracker->moveMarkers->end();
			++i
		) {
			for (std::map<
					sf::Vector2i, MoveMarker*, VectorUtils::cmpVectorLexicographically
				>::iterator j = i->second->begin();
				j != i->second->end();
				++j
			) {
				if (!j->second->canMove(game->controller->curTeamHasMoved()) && !displayDebugData) continue;
				drawTile(j->first.x, j->first.y, MOVE_MARKER_COLOR);
			}
		}
    }

    // Draw mouse overlay

	// If no piece is selected, only pieces should be selectable
    if (selectedPiece == nullptr) {
		Piece* hoveredPiece = game->pieceTracker->getPiece(mousePos);

		// Only the piece on the current turn's team is selectable
		if (hoveredPiece == nullptr || !game->controller->canMove(hoveredPiece->team)) {
			drawTile(mousePos.x, mousePos.y, MOUSE_INVALID_COLOR);
		} else {
			drawTile(mousePos.x, mousePos.y, MOUSE_VALID_COLOR);
		}

	// If a piece is selected, only valid move positions should be selectable
    } else {
		if (game->pieceTracker->getValidMove(selectedPiece, mousePos) != nullptr) {
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
void Renderer::drawPiece(Piece* p) const {
	// Get the team color
	sf::Color teamColor = sf::Color::White;
	std::map<
		const unsigned int,
		std::pair<const std::string,
		sf::Color>
	>::iterator teamIter = teams->find(p->team);
	if (teamIter != teams->end()) {
		teamColor = teamIter->second.second;
	}

	std::map<std::string, sf::Texture*>::iterator it = textures->find(p->pieceDef->name);
	if (it == textures->end() || it->second == nullptr) {
		sf::CircleShape s(tileSize / 2);
		s.setFillColor(teamColor);

		s.setPosition(
			tileStartPos.x + tileSize * (p->pos.x - cameraPos.x + dimensionsInTiles.x / 2),
			tileStartPos.y + tileSize * (p->pos.y - cameraPos.y + dimensionsInTiles.y / 2)
		);

		window->draw(s);
	} else {
        sf::Sprite s;
        s.setTexture(*(it->second));
        s.setColor(teamColor);
        s.setScale(sf::Vector2f(tileSize / 16, tileSize / 16));
        s.setPosition(
			tileStartPos.x + tileSize * (p->pos.x - cameraPos.x + dimensionsInTiles.x / 2),
			tileStartPos.y + tileSize * (p->pos.y - cameraPos.y + dimensionsInTiles.y / 2)
		);

        window->draw(s);
	}
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

	Piece* selectedPiece = game->controller->getSelectedPiece();
	if (selectedPiece == nullptr) {
		s = "Selected piece: null";
		drawDebugText(s, row++);
	} else {
		s = "Selected piece: " + selectedPiece->pieceDef->name;
		drawDebugText(s, row++);
		s = "Selected piece dir: " + std::to_string(selectedPiece->dir);
		drawDebugText(s, row++);
		s = "Selected piece pos: (" + std::to_string(selectedPiece->pos.x) + ", " + std::to_string(selectedPiece->pos.y) + ")";
		drawDebugText(s, row++);
		s = "Selected piece team: " + teams->find(selectedPiece->team)->second.first;
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
	window{w},
	textures{nullptr},
	uiTextures{nullptr},
	teams{nullptr}
{
	window->setFramerateLimit(MAX_FRAMERATE);
}

/**
 * Destructor
 */
Renderer::~Renderer() {
	// Delete all the textures
	if (textures != nullptr) {
		for (std::map<std::string, sf::Texture*>::iterator i = textures->begin(); i != textures->end(); ++i) {
			if (i->second != nullptr) {
				delete i->second;
			}
		}

		// Clean up
		delete textures;
		textures = nullptr;
	}

	// Delete the teams
    if (teams != nullptr) {
        delete teams;
		teams = nullptr;
    }

	game   = nullptr;
	window = nullptr;
}



// Public event handlers

/**
 * Handle initialization
 */
void Renderer::onStartup(
	std::map<std::string, sf::Texture*>* textures_,
	std::map<std::string, sf::Texture*>* uiTextures_,
	std::map<const unsigned int, std::pair<const std::string, sf::Color>>* teams_
) {
	textures = textures_;
	uiTextures = uiTextures_;
	teams = teams_;

	// Load resources
	debugFont.loadFromFile(FONT_DIRECTORY + DEBUG_FONT_FILENAME);

	// Set up window icon
	std::map<std::string, sf::Texture*>::iterator iconIter = textures->find(WINDOW_ICON);
	if (iconIter != textures->end()) {
		sf::Texture* t = iconIter->second;
		window->setIcon(t->getSize().x, t->getSize().y, t->copyToImage().getPixelsPtr());
	}

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

bool Renderer::shouldGenerate(sf::Vector2i baseVector, sf::Vector2i pos) const {
	const sf::Vector2i screenPos = getScreenPos(pos);

	const int leftBoundary = -tileSize;
	const int rightBoundary = (int) window->getSize().x;
	const int topBoundary = -tileSize;
	const int bottomBoundary = (int) window->getSize().y;

	const bool insideX =
		(baseVector.x < 0 && screenPos.x > leftBoundary) ||
		(baseVector.x > 0 && screenPos.x < rightBoundary);

	const bool insideY =
		(baseVector.y < 0 && screenPos.y > topBoundary) ||
		(baseVector.y > 0 && screenPos.y < bottomBoundary);

	return insideX || insideY;
}

/**
 * Determine whether a move marker should generate another move marker
 */
bool Renderer::shouldGenerate(const MoveMarker* terminal) const {
	return shouldGenerate(terminal->getBaseVector(), terminal->getNextPos());
}

/**
 * Determine whether a move marker should be deleted
 */
bool Renderer::shouldDelete(const MoveMarker* terminal) const {
	return
		(terminal->getPrev() != nullptr) &&
		!(shouldGenerate(terminal->getBaseVector(), terminal->getPrev()->getPos()));
}

/**
 * Toggle the menu
 */
void Renderer::toggleMenu() {
	displayMenu ^= 1;
	needsRedraw = true;
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
	drawOverlays();
	drawPieces();
	drawUILayers();

	// Draw debug data
	if (displayDebugData) {
		drawDebug();
	}

	window->display();
	needsRedraw = false;
}
