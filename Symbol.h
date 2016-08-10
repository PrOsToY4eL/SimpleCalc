#pragma once
#include "SimpleElement.h"
class Symbol :
	public virtual SimpleElement
{
private:
	char value;
	Fixity associativity;
public:
	explicit Symbol() = default;
	explicit Symbol(char val, Fixity assoc) : value(val), associativity(assoc) {}
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
	Fixity getFixity()const override { return associativity; }
	virtual ~Symbol() = default;
};
