#include "Expression.h"
void Expression::parse()
{
	enum class PartOfNumber { Integer, Fraction } partOfNumber{ PartOfNumber::Integer };
	const auto base{ 10 };
	auto baseWeight{ 1 };
	auto currentNumber{ 0.0 };
	auto insertNumber{ false };
	auto lastSymbol{ '\0' };
	for (const auto &s : inputData)
	{
		if (s == ' ')
			continue;
		if (s >= '0' && s <= '9')
		{
			if (partOfNumber == PartOfNumber::Integer)
				currentNumber = currentNumber*base + s - 48;
			else
				currentNumber += (s - 48.) / baseWeight;
			baseWeight *= base;
			insertNumber = true;
		}
		else if (s == '.' || s == ',')
		{
			partOfNumber = PartOfNumber::Fraction;
			baseWeight = base;
		}
		else if (s == '+' || s == '-' || s == '*'
			|| s == '/' || s == '^' || s == '!')
		{
			if (insertNumber == true)
			{
				processedInputData.push_back(std::make_shared<Number>(Number{ currentNumber }));
				partOfNumber = PartOfNumber::Integer;
				currentNumber = 0.0;
				baseWeight = 1;
				insertNumber = false;
			}
			if ((processedInputData.empty() || lastSymbol == '(') && (s == '+' || s == '-') || s == '^')
				processedInputData.push_back(std::make_shared<Symbol>(Symbol{ s, SimpleElement::Fixity::right }));
			else processedInputData.push_back(std::make_shared<Symbol>(Symbol{ s, SimpleElement::Fixity::left }));
		}
		else if (s == '(' || s == ')')
		{
			if (insertNumber == true)
			{
				processedInputData.push_back(std::make_shared<Number>(Number{ currentNumber }));
				partOfNumber = PartOfNumber::Integer;
				currentNumber = 0.0;
				baseWeight = 1;
				insertNumber = false;
			}
			processedInputData.push_back(std::make_shared<Symbol>(Symbol{ s, SimpleElement::Fixity::right }));
		}
		else throw exceptionClassError{ "Входные данные не корректны!\n" };
		lastSymbol = s;
	}
	if (insertNumber == true)
		processedInputData.push_back(std::make_shared<Number>(Number{ currentNumber }));
}
void Expression::getRpnData()
{
	std::stack<std::shared_ptr<SimpleElement>> rpnStack;
	for (const auto &element : processedInputData)
	{
		if (!element->isSymbol())
		{
			rpnProcessedInputData.push_back(element);
			continue;
		}
		if (element->getSymbolValue() == '(')
		{
			rpnStack.push(element);
			continue;
		}
		if (element->getSymbolValue() == ')')
		{
			while (!rpnStack.empty() && rpnStack.top()->getSymbolValue() != '(')
			{
				rpnProcessedInputData.push_back(rpnStack.top());
				rpnStack.pop();
			}
			if (!rpnStack.empty())
			{
				rpnStack.pop();
				if (!rpnStack.empty() && !rpnStack.top()->isSymbol())
				{
					rpnProcessedInputData.push_back(rpnStack.top());
					rpnStack.pop();
				}
				continue;
			}
			else throw exceptionClassError{ "Неверно поставлен разделитель или не согласованы скобки.\n" };
		}
		if (element->isSymbol())
		{
			while (!rpnStack.empty()
				&& (element->getFixity() == SimpleElement::Fixity::right && element->priority() < rpnStack.top()->priority()
					|| element->getFixity() == SimpleElement::Fixity::left && element->priority() <= rpnStack.top()->priority()))
			{
				rpnProcessedInputData.push_back(rpnStack.top());
				rpnStack.pop();
			}
			rpnStack.push(element);
			continue;
		}
	}
	while (!rpnStack.empty())
	{
		if (rpnStack.top()->isSymbol())
		{
			rpnProcessedInputData.push_back(rpnStack.top());
			rpnStack.pop();
		}
		else throw exceptionClassError{ "Не согласованы скобки.\n" };
	}
}
void Expression::getResult()
{
	std::stack<std::shared_ptr<SimpleElement>> stack;
	for (const auto &element : rpnProcessedInputData)
	{
		if (!element->isSymbol())
		{
			stack.push(element);
			continue;
		}
		switch (element->getSymbolValue())
		{
		case '+':
		{
			if (element->getFixity() == SimpleElement::Fixity::left)
			{
				auto a = stack.top()->getNumberValue(); stack.pop();
				auto b = stack.top()->getNumberValue();	stack.pop();
				stack.push(std::make_shared<Number>(Number{ b + a }));
			}
			break;
		}
		case '-':
		{
			if (element->getFixity() == SimpleElement::Fixity::left)
			{
				auto a = stack.top()->getNumberValue();	stack.pop();
				auto b = stack.top()->getNumberValue();	stack.pop();
				stack.push(std::make_shared<Number>(Number{ b - a }));
			}
			else
			{
				auto a = stack.top()->getNumberValue(); stack.pop();
				stack.push(std::make_shared<Number>(Number{ -a }));
			}
			break;
		}
		case '*':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Number>(Number{ b * a }));
			break;
		}
		case '/':
		{
			auto a = stack.top()->getNumberValue();	stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Number>(Number{ b / a }));
			break;
		}
		case '^':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Number>(Number{ pow(b, a) }));
			break;
		}
		default:
			throw exceptionClassError{ "Неизвесный оператор.\n" };
		}
	}
	finalResult = stack.top()->getNumberValue();
}

void Expression::clear()
{
	this->finalResult = 0;
	this->inputData.clear();
	this->processedInputData.clear();
	this->rpnProcessedInputData.clear();
}

std::istream& operator>>(std::istream &input, Expression &expressionObject)
{
	expressionObject.clear();
	std::cout << "Введите арифметическое выражение: ";
	getline(input, expressionObject.inputData);
	if (expressionObject.inputData.empty())
		throw Expression::exceptionClassExit{};
	return input;
}
std::ostream& operator<<(std::ostream &output, Expression &expressionObject)
{
	output << "\nРезультат равен: " << expressionObject.finalResult << std::endl;
	return output;
}
