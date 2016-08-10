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
	std::string inputData;
	std::vector<std::shared_ptr<SimpleElement>> processedInputData;
	std::vector<std::shared_ptr<SimpleElement>> rpnProcessedInputData;
	long double finalResult;
public:
	class exceptionClassError
	{
	public:
		std::string errorMsg;
		explicit exceptionClassError() :errorMsg("Неизвестная ошибка.\n") {}
		explicit exceptionClassError(std::string err) :errorMsg(err) {}
	};
	class exceptionClassExit
	{
	public:
		exceptionClassExit() {}
	};
	Expression() = default;
	void parse();
	void getRpnData();
	void getResult();
	void clear();
	friend std::istream& operator>>(std::istream &input, Expression &expression_object);
	friend std::ostream& operator<<(std::ostream &output, Expression &expression_object);
};