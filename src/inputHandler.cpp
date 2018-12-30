#include "inputHandler.h"

// Private methods

/**
 * Handle continuous keyboard input
 */
void InputHandler::checkKeyboard() {
	sf::Vector2f translationVec;

	// Handle camera movement
	if (sf::Keyboard::isKeyPressed(KEY_MOVE_UP  ))  translationVec.y -= MOVE_STEP_SIZE;
	if (sf::Keyboard::isKeyPressed(KEY_MOVE_D0WN))  translationVec.y += MOVE_STEP_SIZE;
	if (sf::Keyboard::isKeyPressed(KEY_MOVE_LEFT))  translationVec.x -= MOVE_STEP_SIZE;
	if (sf::Keyboard::isKeyPressed(KEY_MOVE_RIGHT)) translationVec.x += MOVE_STEP_SIZE;

	// Only move the camera if there is a non-zero displacement
	if (translationVec.x != 0 || translationVec.y != 0) {
		renderer->moveCamera(translationVec);
	}
}

/**
 * Check for events to handle
 */
void InputHandler::checkEvents() {
	sf::Event event;
	while (window->pollEvent(event)) {
        // Only process events if the window is focused or the event is critical
        if (!window->hasFocus() && !isCritical(event.type)) continue;

		switch (event.type) {

		// Check whether the window needs to be closed
		case sf::Event::Closed:
			window->close();
			break;

        // Check whether the window gained focus
        case sf::Event::GainedFocus:
            renderer->draw();
            break;

		// Check whether the dimensions need to be updated
		case sf::Event::Resized:
			renderer->onResize(event.size.width, event.size.height);
			break;

		// Check whether the tile size needs to be updated
		case sf::Event::MouseWheelScrolled:
			renderer->onZoom(event.mouseWheelScroll.delta);
			break;

		// Check whether a key was pressed
		case sf::Event::KeyPressed:
			onKeyPress(event.key);
			break;

		// Check whether the mouse was moved
		case sf::Event::MouseMoved:
			renderer->needsRedraw = true;
			break;

		// Check whether the mouse was clicked
		case sf::Event::MouseButtonPressed:
            game->controller->onMousePress(renderer->getMousePosition());
            break;

		default:
			break;
		}
	}
}

/**
 * Handle keyboard input
 */
void InputHandler::onKeyPress(sf::Event::KeyEvent keyEvent) {
	// Toggle the debug display
	if      (keyEvent.code == KEY_DEBUG) renderer->toggleDisplayDebugData();
	// Close the window
	else if (keyEvent.code == KEY_EXIT ) window->close();
}

/**
 * Check whether an event is critical
 */
bool InputHandler::isCritical(sf::Event::EventType eventType) {
    switch (eventType) {
    // Check whether the event is critical
    case sf::Event::GainedFocus:
    case sf::Event::Closed:
	case sf::Event::Resized:
        return true;
    default:
        return false;
    }
}


// Public constructors

/**
 * Constructor
 *
 * @param w the window
 * @param r the renderer for the window
 */
InputHandler::InputHandler(Game* g, sf::RenderWindow* w, Renderer* r) :
	game{g},
	window{w},
	renderer{r}
{
}

/**
 * Destructor
 */
InputHandler::~InputHandler() {
	game     = nullptr;
	window   = nullptr;
	renderer = nullptr;
}



// Public methods

void InputHandler::tick() {
	// Only process keyboard input if the window is focused
	if (window->hasFocus()) {
        checkKeyboard();
	}

	checkEvents();
}
