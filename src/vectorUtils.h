#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <SFML/Graphics.hpp>

class VectorUtils {
public:
	// Comparator
	struct cmpVectorLexicographically {
		bool operator()(const sf::Vector2i a, const sf::Vector2i b) const {
			return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
		}
	};
};

#endif // VECTOR_UTILS_H
