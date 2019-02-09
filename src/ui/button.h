#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include <functional>
#include <string>

class Button : public virtual Clickable {
private:
	// Members
	sf::Vector2u pos;
	sf::Vector2u dimensions;

	std::string text;
	std::string textureName;
	std::function<void()> action;

public:
	// Constructor
    Button (
		unsigned int xPos, unsigned int yPos, unsigned int width, unsigned int height,
		std::string text_, std::string textureName_, std::function<void()> action_
	):
		pos{xPos, yPos},
		dimensions{width, height},
		text{text_},
		textureName{textureName_},
		action{action_}
	{}

	// Clickable implementation

	virtual bool isClicked(sf::Vector2i clickedPos) const {
		sf::Vector2u clickedPosU(
			((unsigned int)(clickedPos.x - (int)(pos.x))),
			((unsigned int)(clickedPos.y - (int)(pos.y)))
		);

        return
			clickedPosU.x > 0 && clickedPosU.x < dimensions.x &&
			clickedPosU.y > 0 && clickedPosU.y < dimensions.y;
	}

	virtual void onClick() {
		action();
	}

	virtual void draw(
		const std::map<std::string, sf::Texture*>* textures,
		const sf::Font font,
		sf::RenderWindow* window
	) {
		sf::RectangleShape s(sf::Vector2f(dimensions.x, dimensions.y));
		s.setPosition(pos.x, pos.y);

		std::map<std::string, sf::Texture*>::const_iterator i = textures->find(textureName);
		if (i == textures->end()) {
			s.setFillColor(sf::Color::White);
		} else {
			s.setTexture(i->second);
		}
        window->draw(s);

        sf::Text t(text, font, 20);
        t.setOrigin(t.getLocalBounds().width / 2., t.getLocalBounds().height);
        t.setPosition(pos.x + dimensions.x / 2., pos.y + dimensions.y / 2.);
        t.setFillColor(sf::Color::Black);
        window->draw(t);
	}
};

#endif // UI_BUTTON_H
