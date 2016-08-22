#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <memory>
#include <iomanip>
#include "Token.h"
#include "Operand.h"
#include "Operation.h"
#include "AST.h"
class Expression
{
private:
	std::string inputExpression;
	std::vector<std::shared_ptr<Token>> processedInputExpression;
	std::vector<std::shared_ptr<Token>> rpnProcessedInputExpression;
	long double ans;
public:
	AST ast;
	struct LogicError :
		public std::logic_error
	{
		explicit LogicError() :logic_error{ "Unknown error.\n" } {}
		explicit LogicError(const std::string &error) :logic_error{ error } {}
	};
	struct Quit :
		public std::exception
	{
		explicit Quit() :std::exception{ "Quiting...\n" }{};
	};
	explicit Expression() = default;
	void parse();
	bool isOperation(const char &)const;
	long double stringToDouble(const std::string &)const;
	void getRpnData();
	void getAst();
	void getAns();
	void clear();
	friend std::istream& operator>>(std::istream &, Expression &);
	friend std::ostream& operator<<(std::ostream &, Expression &);
	~Expression() = default;
};
