#include "LiFiFileIO.hpp"

#include <iostream>

using LiFiFileIO::WriteFile;
using LiFiFileIO::ReadFile;

using LiFiData::Bitset;

int main()
{
	Bitset testData(5);
	testData.set(1);
	testData.set(3);
	testData.set(4);
	
	WriteFile write("test.lifi");
	write.writeBitset(testData);
	write.close();
	
	ReadFile read("test.lifi");
	Bitset received = read.readFileAsBitset();
	read.close();
	
	std::cout << "Read " << received.asString() << "\n";
	return 0;
}