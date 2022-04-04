#include "Bitset.hpp"

#include <iostream>

using LiFiData::Bitset;

int main()
{
	Bitset testData(64);
	//for (int i = 0; i < 64; i++) testData.set(i);
	std::cout << "Binary string: " << testData.asString() << '\n';
	std::cout << "ASCII string : " << testData.asASCIIString() << '\n';
	std::cout << "Hex string   : " << testData.asHexString() << '\n';
	return 0;
}