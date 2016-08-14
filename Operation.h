#pragma once
#include "Token.h"
class Operation :
	public virtual Token
{
private:
	char value;
	Arity arity;
	Fixity fixity;
public:
	explicit Operation() = default;
	explicit Operation(char val, Arity ar, Fixity fix) : value{ val }, arity{ ar }, fixity { fix } {}
	char getSymbolValue()const override { return value; }
	int priority()const override
	{
		switch (value)
		{
		case '^':
			return 9;
		case '*':
		case '/':
			return 7;
		case '+':
		case '-':
			return 6;
		default:
			return 0;
		}
	}
	long double getNumberValue()const override { return 0; }
	bool isSymbol()const override { return true; }
	Fixity getFixity()const override { return fixity; }
	Arity getArity()const override { return arity; }
	virtual ~Operation() = default;
};
