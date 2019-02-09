#ifndef UI_CLICKABLE_H
#define UI_CLICKABLE_H

#include <SFML/Graphics.hpp>

class Clickable {
public:
	bool consumesClick = true;

	// Check for click
	virtual bool isClicked(sf::Vector2i clickedPos) const = 0;

    // Event handler
    virtual void onClick() = 0;

    // Draw
    virtual void draw(
		const std::map<std::string, sf::Texture*>* textures,
		const sf::Font font,
		sf::RenderWindow* window
	) = 0;
};

#endif // UI_CLICKABLE_H
