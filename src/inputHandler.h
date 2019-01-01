#ifndef CHESS_INPUT_HANDLER_H
#define CHESS_INPUT_HANDLER_H

#include <SFML/Graphics.hpp>
#include "renderer.h"

// Forward declarations
class Game;
class Renderer;



// Class declaration
class InputHandler {
private:
	// Configuration constants
	const float MOVE_STEP_SIZE = 0.005f;

	// Movement keybinds
	const sf::Keyboard::Key KEY_MOVE_UP    = sf::Keyboard::Key::W;
	const sf::Keyboard::Key KEY_MOVE_D0WN  = sf::Keyboard::Key::S;
	const sf::Keyboard::Key KEY_MOVE_LEFT  = sf::Keyboard::Key::A;
	const sf::Keyboard::Key KEY_MOVE_RIGHT = sf::Keyboard::Key::D;

	// Options keybinds
	const sf::Keyboard::Key KEY_DEBUG = sf::Keyboard::Key::F3;
	const sf::Keyboard::Key KEY_EXIT  = sf::Keyboard::Key::Escape;

	// Members
	Game* game;
	sf::RenderWindow* window;
	Renderer* renderer;

	// Methods
	void checkKeyboard();
	void checkEvents();
	void onKeyPress(sf::Event::KeyEvent keyEvent);
	bool isCritical(sf::Event::EventType eventType);

public:
	// Constructors
	InputHandler(Game* g, sf::RenderWindow* w, Renderer* r);
	~InputHandler();

	// Methods
	void tick();
};

#endif // CHESS_INPUT_HANDLER_H
