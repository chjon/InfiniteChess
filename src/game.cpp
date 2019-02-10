#include "game.h"

#include <iostream>
#include "io/boardLoader.h"
#include "io/pieceDefLoader.h"
#include "io/resourceLoader.h"
#include "utils/stringUtils.h"
#include "ui/windowLayer.h"
#include "ui/button.h"

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
		pieceDefs = PieceDefLoader::loadPieceDefs("res/pieces.def");
		textures = ResourceLoader::loadTextures(
			ResourceLoader::getListFromMap(pieceDefs,
				(std::string(*)(const std::string, const PieceDef*)) [](auto name, auto pieceDef){
					return name;
				}
			), "res/textures/pieces/", ".png"
		);

		// Create menu layer
		std::vector<std::string>* uiTextureNames = new std::vector<std::string>();
		WindowLayer* menuLayer = new WindowLayer();
		uiTextureNames->push_back("button");
		Button* button = new Button(0, 0, 80, 40, "Load", "button", [&, this]{ loadBoard("saves/input.chess"); });
		menuLayer->addClickable(button);
		button = new Button(0, 50, 80, 40, "Save", "button", [&, this]{ saveBoard("saves/output.chess"); });
		menuLayer->addClickable(button);
		button = new Button(0, 100, 80, 40, "Exit", "button", [&, this]{ window->close(); });
		menuLayer->addClickable(button);
		uiTextures = ResourceLoader::loadTextures(uiTextureNames, "res/textures/ui/", ".png");

		loadBoard("saves/input.chess");

		inputHandler->addLayer(menuLayer);
		renderer->addLayer(menuLayer);

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

// Helpers
void Game::loadBoard(std::string fileName) {
	std::tuple<
		std::map<const unsigned int, std::pair<const std::string, sf::Color>>*,
		unsigned int,
		std::map<sf::Vector2i, Piece*, VectorUtils::cmpVectorLexicographically>*
	> board = BoardLoader::loadBoard(fileName, pieceDefs);

	// Initialize everything

	pieceTracker->onStartup(pieceDefs, std::get<2>(board));
	renderer->onStartup(textures, uiTextures, std::get<0>(board));
	controller->onStartup(std::get<0>(board), std::get<1>(board));
}

void Game::saveBoard(std::string fileName) {
    std::string toWrite = "[" +
		controller->teamsToString() + "," +
		std::to_string(controller->getCurTurn()) + "," +
		pieceTracker->piecesToString() + ",\n" +
	"]";

	ResourceLoader::write(fileName, toWrite);
}
