#ifndef CHESS_NUM_RULE_H
#define CHESS_NUM_RULE_H

#include <string>

// Class declaration
class NumRule {
public:
	enum Operation {
		EQ, NE, LT, LE, GE, GT
	};

private:
	// Members
    Operation operation;
    unsigned int num;

public:
    // Constructors
    NumRule(const std::string& s);
    ~NumRule();

    // Methods
    bool matches(unsigned int candidate) const;
};

#endif // CHESS_NUM_RULE_H
