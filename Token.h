#pragma once
class Token
{
public:
	enum class Fixity { Left, Right };
	enum class Arity { Nullary, Unary, Binary, Ternary };
	explicit Token() = default;
	virtual long double getNumberValue()const = 0;
	virtual char getSymbolValue()const = 0;
	virtual bool isSymbol()const = 0;
	virtual int priority()const = 0;
	virtual Fixity getFixity()const = 0;
	virtual Arity getArity()const = 0;
	virtual ~Token() = default;
};
