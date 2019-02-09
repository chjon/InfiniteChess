#ifndef UI_WINDOW_LAYER_H
#define UI_WINDOW_LAYER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "clickable.h"

class WindowLayer {
private:
    std::vector<Clickable*> clickables;

public:
	// Methods
    void addClickable(Clickable* clickable) {
    	if (clickable != nullptr) {
			clickables.push_back(clickable);
    	}
    }

    bool handleClick(sf::Vector2i pos) {
    	for (std::vector<Clickable*>::iterator i = clickables.begin(); i != clickables.end(); ++i) {
            if ((*i)->isClicked(pos)) {
				(*i)->onClick();

				if ((*i)->consumesClick) {
					return true;
				}
            }
    	}

    	return false;
    }

    virtual void draw(
		const std::map<std::string, sf::Texture*>* textures,
		const sf::Font font,
		sf::RenderWindow* window
	) {
		for (std::vector<Clickable*>::iterator i = clickables.begin(); i != clickables.end(); ++i) {
            (*i)->draw(textures, font, window);
    	}
    }
};

#endif // UI_WINDOW_LAYER_H
