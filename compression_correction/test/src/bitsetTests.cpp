#include "Bitset.hpp"

#include <iostream>

using LiFiData::Bitset;
using LiFiData::hammingDistance;
using LiFiData::stringType;

void hammingDistanceCorrect()
{
	Bitset a(5), b(5);
	a.set(0);
	if (hammingDistance(a,b) != 1) std::cout << "Hamming Distance incorrect!\n";
}

void makeBitsetFromBinaryString()
{
	Bitset a("00100", stringType::BINARY_STRING);
	if (a.getLength() != 5) std::cout << "fromBinaryString length incorrect: " << a.asString() << "\n";
	if (!a[2]) std::cout << "fromBinaryString value incorrect: " << a.asString() << "\n";
}

void makeBitsetFromHexString()
{
	Bitset a("28", stringType::HEX_STRING);
	if (a.getLength() != 8) std::cout << "fromHexString length incorrect: " << a.asString() << "\n";
	if (!a[3]) std::cout << "fromHexString value incorrect: " << a.asHexString() << "\n";
}

void makeBitsetFromASCIIString()
{
	Bitset a("abc", stringType::ASCII_STRING);
	if (a.getLength() != 24) std::cout << "fromASCIIString length incorrect: " << a.asString() << "\n";
}

void concatBitsetCorrectLength()
{
	Bitset a(5), b(5);
	if ((a+b).getLength() != 10) std::cout << "Concat Bitset bad length: " << (a+b).asString() << "\n";	
}

void concatBitsetCorrectValues()
{
	Bitset a(5), b(5);
	a.set(1);
	if (!(a+b)[1]) std::cout << "Concat Bitset bad value: " << (a+b).asString() << "\n";
}

void asStringGood()
{
	Bitset a(5);
	if (a.asString() != "00000") std::cout << "asString bad: " << a.asString() << '\n';
	a.set(2);
	if (a.asString() != "00100") std::cout << "asString bad: " << a.asString() << '\n';
}

void asHexStringGood()
{
	Bitset a(8);
	if (a.asHexString() != "00") std::cout << "asHexString bad: " << a.asHexString() << '\n';
	a.set(2);
	if (a.asHexString() != "04") std::cout << "asHexString bad: " << a.asHexString() << '\n';
}

int main(int argc, char** argv)
{
	hammingDistanceCorrect();
	makeBitsetFromBinaryString();
	makeBitsetFromHexString();
	makeBitsetFromASCIIString();
	concatBitsetCorrectLength();
	concatBitsetCorrectValues();
	asStringGood();
	asHexStringGood();

	return 0;
}