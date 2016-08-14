#include "Expression.h"
void Expression::parse()
{
	if (inputExpression.find("exit") != std::string::npos)
		throw Exit{};
	while (inputExpression.find(' ') != std::string::npos)
		inputExpression.erase(inputExpression.find(' '), 1);
	std::string tempString{};
	auto index{ 0U };
	auto inputExpressionSize{ inputExpression.size() };
	auto isCorrectIndex = [&index, inputExpressionSize]() {return inputExpressionSize > index; };
	while (isCorrectIndex())
	{
		while (isCorrectIndex() && (inputExpression[index] == 'a' || inputExpression[index] == 'n' || inputExpression[index] == 's'))
		{
			tempString += inputExpression[index++];
			if (tempString == "ans")
			{
				tempString.erase();
				processedInputExpression.push_back(std::make_shared<Operand>(*new Operand{ this->ans }));
			}
		}
		if (!tempString.empty())
			throw LogicError{ "Lexical error: Expression is incorrect.\n" };
		while (isCorrectIndex() && inputExpression[index] >= '0' && inputExpression[index] <= '9'
			|| inputExpression[index] == '.' || inputExpression[index] == ',')
			tempString += (inputExpression[index++] == ',') ? '.' : inputExpression[index - 1];
		if (!tempString.empty())
		{
			if ((tempString.find_first_of('.') != std::string::npos)
				&& (tempString.find_first_of('.') != tempString.find_last_of('.')))
				throw LogicError{ "Lexical error: Expression is incorrect.\n" };
			processedInputExpression.push_back(std::make_shared<Operand>(*new Operand{ stringToDouble(tempString) }));
			tempString.erase();
		}
		if (isCorrectIndex())
			if (isOperation(inputExpression[index]))
			{
				if ((processedInputExpression.empty() || processedInputExpression.back()->getSymbolValue() == '(') &&
					(inputExpression[index] == '+' || inputExpression[index] == '-'))
					processedInputExpression.push_back(std::make_shared<Operation>(*new Operation{ inputExpression[index], Token::Arity::Unary, Token::Fixity::Right }));
				else if (inputExpression[index] == '^')
					processedInputExpression.push_back(std::make_shared<Operation>(*new Operation{ inputExpression[index], Token::Arity::Binary, Token::Fixity::Right }));
				else
					processedInputExpression.push_back(std::make_shared<Operation>(*new Operation{ inputExpression[index], Token::Arity::Binary, Token::Fixity::Left }));
			}
			else if (inputExpression[index] == '(' || inputExpression[index] == ')')
				processedInputExpression.push_back(std::make_shared<Operation>(*new Operation{ inputExpression[index], Token::Arity::Nullary, Token::Fixity::Right }));
			else throw LogicError{ "Lexical error: Your input is incorrect.\n" };
			++index;
	}
}

bool Expression::isOperation(const char& symbol)
{
	switch (symbol)
	{
	case '+':
	case '-':
	case '*':
	case '/':
	case '^':
	case '!':
		return true;
	default:
		return false;
	}
}

long double Expression::stringToDouble(const std::string &string)
{
	auto integerPart{ true };
	auto number{ 0.L };
	const auto base{ 10 };
	auto baseWeight{ 1 };
	for (const auto& symbol : string)
	{
		if (symbol == '.')
		{
			integerPart = false;
			baseWeight = base;
			continue;
		}
		if (integerPart)
			number = number*base + symbol - 48;
		else
			number += (symbol - 48.) / baseWeight;
		baseWeight *= base;
	}
	return number;
}

void Expression::getRpnData()
{
	std::stack<std::shared_ptr<Token>> rpnStack;
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
			else throw LogicError{ "Syntax error: The separator was misplaced or brackets were mismatched.\n" };
		}
		if (element->isSymbol())
		{
			while (!rpnStack.empty()
				&& (element->getFixity() == Token::Fixity::Right && element->priority() < rpnStack.top()->priority()
					|| element->getFixity() == Token::Fixity::Left && element->priority() <= rpnStack.top()->priority()))
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
		if (rpnStack.top()->isSymbol() && rpnStack.top()->getArity() != Token::Arity::Nullary)
		{
			rpnProcessedInputExpression.push_back(rpnStack.top());
			rpnStack.pop();
		}
		else throw LogicError{ "Syntax error: Brackets are not coordinated.\n" };
	}
}
void Expression::getAns()
{
	std::stack<std::shared_ptr<Token>> stack;
	if (rpnProcessedInputExpression.empty())
		throw LogicError{ "Parsing error: Your input is empty.\n" };
	for (const auto &element : rpnProcessedInputExpression)
	{
		if (!element->isSymbol())
		{
			stack.push(element);
			continue;
		}
		if (element->getArity() == Token::Arity::Unary && stack.empty() ||
			element->getArity() == Token::Arity::Binary && stack.size() < 2)
			throw LogicError{ "Parsing error: Maybe your input is incorrect.\n" };
		switch (element->getSymbolValue())
		{
		case '+':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			if (element->getArity() == Token::Arity::Binary)
			{
				auto b = stack.top()->getNumberValue();	stack.pop();
				stack.push(std::make_shared<Operand>(*new Operand{ b + a }));
			}
			else
				stack.push(std::make_shared<Operand>(*new Operand{ a }));
			break;
		}
		case '-':
		{
			auto a = stack.top()->getNumberValue();	stack.pop();
			if (element->getArity() == Token::Arity::Binary)
			{
				auto b = stack.top()->getNumberValue();	stack.pop();
				stack.push(std::make_shared<Operand>(*new Operand{ b - a }));
			}
			else
				stack.push(std::make_shared<Operand>(*new Operand{ -a }));
			break;
		}
		case '*':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Operand>(*new Operand{ b * a }));
			break;
		}
		case '/':
		{
			auto a = stack.top()->getNumberValue();	stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Operand>(*new Operand{ b / a }));
			break;
		}
		case '^':
		{
			auto a = stack.top()->getNumberValue(); stack.pop();
			auto b = stack.top()->getNumberValue();	stack.pop();
			stack.push(std::make_shared<Operand>(*new Operand{ pow(b, a) }));
			break;
		}
		default:
			throw LogicError{ "Parsing error: Unknown operation.\n" };
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
	do
	{
		std::cout << ">> ";
		getline(input, expressionObject.inputExpression);
	} while (expressionObject.inputExpression.empty());
	return input;
}
std::ostream& operator<<(std::ostream &output, Expression &expressionObject)
{
	output << "ans = " << /*std::setprecision(15) <<*/ expressionObject.ans << /*std::fixed <<*/ std::endl;
	return output;
}
