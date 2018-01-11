#include "DevTestClass.hpp"
#include <iostream>

void DevTestClass::sayHello() const
{
	std::cout << "hello world!\n";
}

int DevTestClass::getInt() const
{
	return VALUE;
}
