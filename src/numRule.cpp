#include "numRule.h"

#include <string>
#include "resourceLoader.h"

// Constructors

NumRule::NumRule(const std::string& s) :
	operation{EQ},
	num{1}
{
	enum State {
		START, EQ, N, L, G
	};

	bool gotState = false;
	State state = START;
    unsigned int index = 0;
    while (index < s.length() && !gotState) {
		char curChar = s[index];

		switch (state) {
		case State::START:
            switch (curChar) {
			case '=':
				state = State::EQ;
				operation = NumRule::Operation::EQ;
				gotState = true;
				break;
			case '!':
				state = State::N;
				break;
			case '<':
				state = State::L;
				break;
			case '>':
				state = State::G;
				break;
			default:
				throw ResourceLoader::FileFormatException("Invalid token '" + std::to_string(curChar) + "'");
            }
            index++;
            break;
		case State::N:
			if (curChar != '=') {
				throw ResourceLoader::FileFormatException("Invalid token '" + std::to_string(curChar) + "'");
			}
			operation = NumRule::Operation::NE;
			gotState = true;
			index++;
			break;
		case State::L:
            if (curChar == '=') {
                operation = NumRule::Operation::LE;
                index++;
            } else {
				operation = NumRule::Operation::LT;
            }
            gotState = true;
            break;
		case State::G:
			if (curChar == '=') {
                operation = NumRule::Operation::GE;
                index++;
            } else {
				operation = NumRule::Operation::GT;
            }
            gotState = true;
            break;
		default:
			throw ResourceLoader::FileFormatException("Invalid state '" + std::to_string(state) + "'");
		}
    }

    num = std::stoi(s.substr(index));
}

NumRule::~NumRule() {
}



// Methods

/**
 * Determine whether a number matches the number rule
 */
bool NumRule::matches(unsigned int candidate) const {
    switch (operation) {
	case EQ:
		return candidate == num;
	case NE:
		return candidate != num;
	case LT:
		return candidate < num;
	case LE:
		return candidate <= num;
	case GT:
		return candidate > num;
	case GE:
		return candidate >= num;
	default:
		return false;
    }
}
