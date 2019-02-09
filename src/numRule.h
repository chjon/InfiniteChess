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

    // Accessors
    inline const Operation getOperation() const { return operation; };
    inline const unsigned int getNum() const { return num; };

    // Methods
    bool matches(unsigned int candidate) const;
};

#endif // CHESS_NUM_RULE_H
