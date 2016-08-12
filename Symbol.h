#pragma once
#include "SimpleElement.h"
class Symbol :
	public virtual SimpleElement
{
private:
	char value;
	Arity arity;
	Fixity fixity;
public:
	Symbol() = default;
	Symbol(char val, Arity ar, Fixity fix) : value{ val }, arity{ ar }, fixity { fix } {}
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
	virtual ~Symbol() = default;
};
