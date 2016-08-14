#pragma once
#include "Token.h"
class Operand :
	public virtual Token
{
private:
	long double value;
public:
	explicit Operand() = default;
	explicit Operand(long double val) : value{ val } {}
	long double getNumberValue()const override { return value; }
	char getSymbolValue()const override { return 0; }
	bool isSymbol()const override { return false; }
	int priority()const override { return 0; }
	Fixity getFixity()const override { return Fixity::Left; }
	Arity getArity()const override { return Arity::Nullary; }
	virtual ~Operand() = default;
};
