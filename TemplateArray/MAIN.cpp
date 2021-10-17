#include "DynamicArray.h"

int main()
{
	dynamic::array<int> intArray;
	dynamic::array<double> doubleArray;
	dynamic::array<char> charArray;

	intArray.add(5);

	std::cout << intArray;

	system("pause>nul");
	return 0;
}