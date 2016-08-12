#include <iostream>
#include "Expression.h"
int main()
{
	setlocale(0, "");
	std::cout << "SimpleCalc by PrOsToY_4eL v0.1.1 " << std::endl;
	auto expression{ std::make_unique<Expression>(*new Expression{}) };
	while (true)
	{
		try
		{
			std::cin >> *expression;
			expression->parse();
			expression->getRpnData();
			expression->getAns();
			std::cout << *expression;
		}
		catch (const Expression::exceptionClassError &exc)
		{
			std::cout << "Внимание! " << exc.errorMsg;
		}
		catch (const Expression::exceptionClassExit&)
		{
			std::cout << "Вы ничего не ввели и нажали Enter.\n";
			break;
		}
		catch (...)
		{
			std::cerr << "Неизесная ошибка";
			break;
		}
	}
	system("pause");
	return 0;
}