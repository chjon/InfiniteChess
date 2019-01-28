#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include <SFML/Graphics.hpp>
#include <string>
#include "stringUtils.h"

class VectorUtils {
public:
	/**
	 * Comparator
	 */
	struct cmpVectorLexicographically {
		bool operator()(const sf::Vector2i a, const sf::Vector2i b) const {
			return (a.x == b.x) ? (a.y < b.y) : (a.x < b.x);
		}
	};

	/**
	 * Rotate 90 degrees CCW
	 */
	inline static void rotate90(sf::Vector2i& vec) {
		const int temp = vec.x;
		vec.x = -vec.y;
		vec.y = temp;
	}

	/**
	 * Rotate 180 degrees CCW
	 */
	inline static void rotate180(sf::Vector2i& vec) {
		vec.x = -vec.x;
		vec.y = -vec.y;
	}

	/**
	 * Rotate 270 degrees CCW
	 */
	inline static void rotate270(sf::Vector2i& vec) {
		const int temp = -vec.x;
		vec.x = vec.y;
		vec.y = temp;
	}

	/**
	 * Determine whether the candidate vector is an integer multiple of the base vector
	 */
	inline static bool isIntegerMultiple(const sf::Vector2i base, const sf::Vector2i candidate) {
		if (base.x != 0 && candidate.x != 0) {
			return candidate.y == (base.y * (candidate.x / base.x));
		} else if (base.x != 0 || candidate.x != 0) {
			return false;
		} else if (base.y != 0 && candidate.y != 0) {
			return candidate.y == (base.y * (candidate.y / base.y));
		} else {
			return base.y == candidate.y;
		}
	}

	/**
	 * Convert the vector to a string
	 */
    inline static std::string toString(const sf::Vector2i v) {
        return "[" + std::to_string(v.x) + "," + std::to_string(v.y) + ",]";
    }

    /**
     * Get a vector from a string
     */
    inline static sf::Vector2i fromString(const std::string& s) {
        std::vector<int>* values = StringUtils::getIntList(s.substr(1, s.length() - 2), ',', '[', ']');
        sf::Vector2i v((*values)[0], (*values)[1]);

        // Clean up and return
        delete values;
        return v;
    }

    /**
	 * Apply reflections to a vector
	 */
	inline static sf::Vector2i reflect(const sf::Vector2i original, bool reflectX, bool reflectY, bool reflectXY) {
		sf::Vector2i reflected = original;
		if (reflectX) reflected.x = -reflected.x;
		if (reflectY) reflected.y = -reflected.y;
		if (reflectXY) std::swap(reflected.x, reflected.y);
		return reflected;
	}
};

#endif // VECTOR_UTILS_H
