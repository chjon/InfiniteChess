#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>

class StringUtils {
public:
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
	static inline std::vector<std::string> split (std::string s, std::string t) {
		unsigned int i = s.find(t, 0);
		unsigned int last = 0;
		std::vector<std::string>* splitString = new std::vector<std::string>();

		while (i != std::string::npos) {
			splitString->push_back(s.substr(last, i - last));
			last = i + (int) t.length();
			i = s.find(t, i + 1);
		};

		splitString->push_back(s.substr(last, s.length() - last));

		return *splitString;
	}
};

#endif // STRING_UTILS_H
