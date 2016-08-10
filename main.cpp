#include <iostream>
#include "Expression.h"
int main()
{
	setlocale(0, "");
	auto expression{ std::make_unique<Expression>() };
	while(true)
	{
		try
		{
			std::cin >> *expression;
			expression->parse();
			expression->getRpnData();
			expression->getResult();
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
	}
	system("pause");
	return 0;
}