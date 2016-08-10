#pragma once

class SimpleElement
{
public:
	enum class Fixity { left, right };
	explicit SimpleElement() = default;
	explicit SimpleElement(const SimpleElement&) = default;
	virtual long double getNumberValue()const = 0;
	virtual char getSymbolValue()const = 0;
	virtual bool isSymbol()const = 0;
	virtual int priority()const = 0;
	virtual Fixity getFixity()const = 0;
	virtual ~SimpleElement() = default;
};
