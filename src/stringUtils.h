#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>

class StringUtils {
public:
	// Exceptions

	class ParseException : public std::exception {
	private:
		const std::string message;
	public:
		// Constructors
		inline ParseException(const std::string m) : message{m} {}
		inline ~ParseException() {}

		// Overridden methods
		inline virtual const char* what() const throw() {
			return message.c_str();
		}
	};

	// trim from start (in place)
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	static inline void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}

	// trim from start (copying)
	static inline std::string ltrim_copy(std::string s) {
		ltrim(s);
		return s;
	}

	// trim from end (copying)
	static inline std::string rtrim_copy(std::string s) {
		rtrim(s);
		return s;
	}

	// trim from both ends (copying)
	static inline std::string trim_copy(std::string s) {
		trim(s);
		return s;
	}

	/**
	 * Split a string by a sequence
	 *
	 * @param s the string to split
	 * @param t the sequence to split on
	 *
	 * @return a vector containing the split strings
	 */
	static inline std::vector<std::string>* split (std::string s, std::string t) {
		unsigned int i = s.find(t, 0);
		unsigned int last = 0;
		std::vector<std::string>* splitString = new std::vector<std::string>();

		while (i != std::string::npos) {
			splitString->push_back(s.substr(last, i - last));
			last = i + (int) t.length();
			i = s.find(t, i + 1);
		};

		splitString->push_back(s.substr(last, s.length() - last));

		return splitString;
	}

	/**
	 * Split the string on the separator while leaving bracket-enclosed groups intact
	 */
	static inline std::vector<std::string>* getList(const std::string& s, const char separator,
		const char bracketOpen, const char bracketClose
	) {
		std::vector<std::string>* splitStrings = new std::vector<std::string>();
		std::string stringSoFar = "";
		int nestLevel = 0;

		// Split the string on the separator
		int lineBegin = 0;
		for (unsigned int i = 0; i < s.length(); i++) {
			const char curChar = s[i];
			if (curChar == bracketOpen) {
				nestLevel++;
			} else if (curChar == bracketClose) {
				// Check if a bracket was expected
				if (nestLevel == 0) {
					throw ParseException(
						"Unexpected token '" + std::to_string(bracketClose) + "'"
					);
				}

				nestLevel--;
			} else if (curChar == separator && nestLevel == 0) {
				// Split
				stringSoFar += s.substr(lineBegin, i - lineBegin);
				lineBegin = i + 1;
				splitStrings->push_back(stringSoFar);
				stringSoFar = "";
			}
		}

		return splitStrings;
	}

	/**
	 * Get a list of integers from a string
	 *
	 * @param s the string from which to generate the list
	 */
	static inline std::vector<int>* getIntList(const std::string& s, const char separator,
		const char bracketOpen, const char bracketClose
	) {
        // Setup
        std::vector<std::string>* stringList = getList(s, separator, bracketOpen, bracketClose);
        std::vector<int>* intList = new std::vector<int>();

        // Generate ints
        for (std::vector<std::string>::iterator i = stringList->begin(); i != stringList->end(); ++i) {
            intList->push_back(std::stoi(*i));
        }

        // Clean up and return
        delete stringList;
        return intList;
	}

	/**
	 * Check whether a character is in a list
	 */
	inline static const bool isInList(const char candidate, const std::vector<char>* list) {
		for (std::vector<char>::const_iterator i = list->begin(); i != list->end(); ++i) {
			if (*i == candidate) return true;
		}

		return false;
	}
};

#endif // STRING_UTILS_H
