#include <iostream>
#include "Expression.h"
int main()
{
	setlocale(0, "");
	std::cout << "******************************************" << std::endl;
	std::cout << "***  SimpleCalc by PrOsToY_4eL v0.1.2  ***" << std::endl;
	std::cout << "******************************************" << std::endl;
	auto expression{ std::make_unique<Expression>(*new Expression{}) };
	while (true)
	{
		try
		{
			std::cin >> *expression;
			expression->parse();
			expression->getRpnData();
			expression->getAns();
			expression->getAst();
			std::cout << *expression;
		}
		catch (const Expression::LogicError &logicError)
		{
			std::cerr << "Warning! " << logicError.what();
		}
		catch (const Expression::Exit &exit)
		{
			std::clog << exit.what();
			break;
		}
		catch (...)
		{
			std::clog << "Unknown exception.\n";
		}
	}
	system("pause");
	return 0;
}
