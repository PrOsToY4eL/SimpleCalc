#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <memory>
#include "SimpleElement.h"
#include "Number.h"
#include "Symbol.h"
class Expression
{
private:
	std::string inputExpression;
	std::vector<std::shared_ptr<SimpleElement>> processedInputExpression;
	std::vector<std::shared_ptr<SimpleElement>> rpnProcessedInputExpression;
	long double ans;
public:
	class exceptionClassError
	{
	public:
		std::string errorMsg;
		explicit exceptionClassError() :errorMsg{ "Неизвестная ошибка.\n" } {}
		explicit exceptionClassError(const std::string &err) :errorMsg{ err } {}
	};
	class exceptionClassExit
	{
	public:
		explicit exceptionClassExit() = default;
	};
	explicit Expression() = default;
	void parse();
	void getRpnData();
	void getAns();
	void clear();
	friend std::istream& operator>>(std::istream &input, Expression &expression_object);
	friend std::ostream& operator<<(std::ostream &output, Expression &expression_object);
	~Expression() = default;
};
