#pragma once
#include "SimpleElement.h"
class Number :
	public virtual SimpleElement
{
private:
	long double value;
public:
	explicit Number() = default;
	explicit Number(long double val) : value(val) {}
	long double getNumberValue()const override { return value; }
	char getSymbolValue()const override { return 0; }
	bool isSymbol()const override { return false; }
	int priority()const override { return 0; }
	Fixity getFixity()const override { return Fixity::left; }
	virtual ~Number() = default;
};
