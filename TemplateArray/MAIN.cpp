#include "DynamicArray.h"

int main()
{
	dynamic::Array<int> intArray;
	dynamic::Array<double> doubleArray;
	dynamic::Array<char> charArray;

	intArray.push_back(5);

	std::cout << intArray;

	system("pause>nul");
	return 0;
}