#include <iostream>
#include <thread>

void test()
{
	std::cout << "hello from test" << std::endl;
}

void functionA()
{
	std::thread threadC(test);
	std::cout << "hello from functionA" << std::endl;
	threadC.join();
}

void functionB()
{
	std::cout << "hello from functionB" << std::endl;
}

int main()
{
	std::thread threadA(functionA);
	std::thread threadB(functionB);

	threadA.join();
	threadB.join();
	return 0;
}
