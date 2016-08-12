#include "Expression.h"
void Expression::parse()
{
	enum class PartOfNumber { Integer, Fraction } partOfNumber{ PartOfNumber::Integer };
	const auto base{ 10 };
	auto baseWeight{ 1 };
	auto currentNumber{ 0.0 };
	auto insertNumber{ false };
	auto lastSymbol{ '\0' };
	std::string ansStr{};
	for (const auto &symbol : inputExpression)
	{
		switch (symbol)
		{
		case 'a':
			if (ansStr.empty())
				ansStr += symbol;
			continue;
			break;
		case 'n':
			if (ansStr == "a")
				ansStr += symbol;
			continue;
			break;
		case 's':
			if (ansStr == "an")
			{
				ansStr.clear();
				processedInputExpression.push_back(std::make_shared<Number>(*new Number{ this->ans }));
			}
			continue;
			break;
		default:
			break;
		}

		if (symbol == ' ')
			continue;
		if (symbol >= '0' && symbol <= '9')
		{
			if (partOfNumber == PartOfNumber::Integer)
				currentNumber = currentNumber*base + symbol - 48;
			else
				currentNumber += (symbol - 48.) / baseWeight;
			baseWeight *= base;
			insertNumber = true;
		}
		else if (symbol == '.' || symbol == ',')
		{
			partOfNumber = PartOfNumber::Fraction;
			baseWeight = base;
		}
		else if (symbol == '+' || symbol == '-' || symbol == '*'
			|| symbol == '/' || symbol == '^' || symbol == '!')
		{
			if (insertNumber == true)
			{
				processedInputExpression.push_back(std::make_shared<Number>(*new Number{ currentNumber }));
				partOfNumber = PartOfNumber::Integer;
				currentNumber = 0.0;
				baseWeight = 1;
				insertNumber = false;
			}
			if ((processedInputExpression.empty() || lastSymbol == '(') && (symbol == '+' || symbol == '-'))
				processedInputExpression.push_back(std::make_shared<Symbol>(*new Symbol{ symbol, SimpleElement::Arity::Unary, SimpleElement::Fixity::Right }));
			else if (symbol == '^')
				processedInputExpression.push_back(std::make_shared<Symbol>(*new Symbol{ symbol, SimpleElement::Arity::Binary, SimpleElement::Fixity::Right }));
			else
				processedInputExpression.push_back(std::make_shared<Symbol>(*new Symbol{ symbol, SimpleElement::Arity::Binary, SimpleElement::Fixity::Left }));
		}
		else if (symbol == '(' || symbol == ')')
		{
			if (insertNumber == true)
			{
				processedInputExpression.push_back(std::make_shared<Number>(*new Number{ currentNumber }));
				partOfNumber = PartOfNumber::Integer;
				currentNumber = 0.0;
				baseWeight = 1;
				insertNumber = false;
			}
			processedInputExpression.push_back(std::make_shared<Symbol>(*new Symbol{ symbol, SimpleElement::Arity::Nullary, SimpleElement::Fixity::Right }));
		}
		else throw exceptionClassError{ "Входные данные не корректны!\n" };
		lastSymbol = symbol;
	}
	if (insertNumber == true)
		processedInputExpression.push_back(std::make_shared<Number>(*new Number{ currentNumber }));
}
void Expression::getRpnData()
{
	std::stack<std::shared_ptr<SimpleElement>> rpnStack;
	for (const auto &element : processedInputExpression)
	{
		if (!element->isSymbol())
		{
			rpnProcessedInputExpression.push_back(element);
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
				rpnProcessedInputExpression.push_back(rpnStack.top());
				rpnStack.pop();
			}
			if (!rpnStack.empty())
			{
				rpnStack.pop();
				if (!rpnStack.empty() && !rpnStack.top()->isSymbol())
				{
					rpnProcessedInputExpression.push_back(rpnStack.top());
					rpnStack.pop();
				}
				continue;
			}
			else throw exceptionClassError{ "Неверно поставлен разделитель или не согласованы скобки.\n" };
		}
		if (element->isSymbol())
		{
			while (!rpnStack.empty()
					&& (element->getFixity() == SimpleElement::Fixity::Right && element->priority() < rpnStack.top()->priority()
					|| element->getFixity() == SimpleElement::Fixity::Left && element->priority() <= rpnStack.top()->priority()))
			{
				rpnProcessedInputExpression.push_back(rpnStack.top());
				rpnStack.pop();
			}
			rpnStack.push(element);
			continue;
		}
	}
	while (!rpnStack.empty())
	{
		if (rpnStack.top()->isSymbol() && rpnStack.top()->getArity() != SimpleElement::Arity::Nullary)
		{
			rpnProcessedInputExpression.push_back(rpnStack.top());
			rpnStack.pop();
		}
		else throw exceptionClassError{ "Не согласованы скобки.\n" };
	}
}
void Expression::getAns()
{
	std::stack<std::shared_ptr<SimpleElement>> stack;
	if (rpnProcessedInputExpression.empty())
		throw exceptionClassError{ "Пустое выражение\n" };
	for (const auto &element : rpnProcessedInputExpression)
	{
		if (!element->isSymbol())
		{
			stack.push(element);
			continue;
		}
		if (element->getArity() == SimpleElement::Arity::Unary && stack.empty() ||
			element->getArity() == SimpleElement::Arity::Binary && stack.size() < 2)
			throw exceptionClassError{ "Ошибка в выражении!\n" };
		switch (element->getSymbolValue())
		{
		case '+':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			if (element->getArity() == SimpleElement::Arity::Binary)
			{
				auto b = stack.top()->getNumberValue();	stack.pop();
				stack.push(std::make_shared<Number>(*new Number{ b + a }));
			}
			else
				stack.push(std::make_shared<Number>(*new Number{ a }));
			break;
		}
		case '-':
		{
			auto a = stack.top()->getNumberValue();	stack.pop();
			if (element->getArity() == SimpleElement::Arity::Binary)
			{
				auto b = stack.top()->getNumberValue();	stack.pop();
				stack.push(std::make_shared<Number>(*new Number{ b - a }));
			}
			else
				stack.push(std::make_shared<Number>(*new Number{ -a }));
			break;
		}
		case '*':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Number>(*new Number{ b * a }));
			break;
		}
		case '/':
		{
			auto a = stack.top()->getNumberValue();	stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Number>(*new Number{ b / a }));
			break;
		}
		case '^':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Number>(*new Number{ pow(b, a) }));
			break;
		}
		default:
			throw exceptionClassError{ "Неизвесный оператор.\n" };
		}
	}
	ans = stack.top()->getNumberValue();
}

void Expression::clear()
{
	this->inputExpression.clear();
	this->processedInputExpression.clear();
	this->rpnProcessedInputExpression.clear();
}

std::istream& operator>>(std::istream &input, Expression &expressionObject)
{
	expressionObject.clear();
	std::cout << ">> ";
	getline(input, expressionObject.inputExpression);
	if (expressionObject.inputExpression.empty())
		throw Expression::exceptionClassExit{};
	return input;
}
std::ostream& operator<<(std::ostream &output, Expression &expressionObject)
{
	output << "ans = " << expressionObject.ans << std::endl;
	return output;
}
