#include "Bitset.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

using LiFiData::Bitset;

std::string convertHexStringToBinary(const std::string& hex)
{
	std::string bin;
	for (char c : hex)
	{
		switch (c)
		{
			case '0':
				bin += "0000";
				break;
			case '1':
				bin += "0001";
				break;
			case '2':
				bin += "0010";
				break;
			case '3':
				bin += "0011";
				break;
			case '4':
				bin += "0100";
				break;
			case '5':
				bin += "0101";
				break;
			case '6':
				bin += "0110";
				break;
			case '7':
				bin += "0111";
				break;
			case '8':
				bin += "1000";
				break;
			case '9':
				bin += "1001";
				break;
			case 'A':
				bin += "1010";
				break;
			case 'B':
				bin += "1011";
				break;
			case 'C':
				bin += "1100";
				break;
			case 'D':
				bin += "1101";
				break;
			case 'E':
				bin += "1110";
				break;
			case 'F':
				bin += "1111";
				break;
		}
	}
	return bin;
}

int LiFiData::hammingDistance(const Bitset& set1, const Bitset& set2)
{
	if (set1.length != set2.length) return 0;
	unsigned int dist = 0;
	for (int i = 0; i < set1.length; i++)
	{
		if (set1[i] != set2[i]) dist++;
	}
	return dist;
}

Bitset::Bitset()
{
	length = 0;
}

void Bitset::fromBinaryString(const std::string& s)
{
	*this = Bitset(s.size());
	for (int i = 0; i < length; i++)
	{
		if (s[length-i-1] == '1') set(i);
	}
}

void Bitset::fromHexString(const std::string& s)
{
	std::string binString = convertHexStringToBinary(s);
	fromBinaryString(binString);
}

void Bitset::fromASCIIString(const std::string& s)
{
	length = s.size()*8;
	data = std::vector<uint8_t>(s.begin(), s.end());
}

Bitset::Bitset(const std::string& s, stringType type) : length(0)
{
	switch (type)
	{
		case BINARY_STRING:
			fromBinaryString(s);
			break;
		case HEX_STRING:
			fromHexString(s);
			break;
		case ASCII_STRING:
			fromASCIIString(s);
			break;
	}
}

bool Bitset::badIndex(int i) const { return i < 0 || i >= length; }

Bitset::Bitset(const std::vector<uint8_t>& raw) : data(raw), length(raw.size()*8) {}

Bitset::Bitset(const std::vector<uint8_t>& raw, int l) : data(raw), length(l) {}

Bitset::Bitset(int l) : length(l)
{
	for (int i = 0; i < l/8+(l%8)?1:0; i++) data.push_back(0);
}

const Bitset Bitset::operator+(const Bitset& rhs) const
{
	Bitset combined(length+rhs.length);
	
	for (int i = 0; i < length; i++)
	{
		if (at(i)) combined.set(i);
	}
	for (int i = 0; i < rhs.length; i++)
	{
		if (rhs.at(i)) combined.set(i+length);
	}
	return combined;
}

bool Bitset::operator[](int i) const
{
	return at(i);
}

bool Bitset::at(int i) const
{
	if (badIndex(i)) return false;
	return data[i/8] & (0x1<<(i%8));
}

void Bitset::set(int i)
{
	if (badIndex(i)) return;
	data[i/8] |= (0x1<<(i%8));
}

void Bitset::toggle(int i)
{
	if (badIndex(i)) return;
	data[i/8] ^= (0x1<<(i%8));
}

void Bitset::clear(int i)
{
	if (badIndex(i)) return;
	data[i/8] &= ~(0x1<<(i%8));
}

Bitset Bitset::getSubset(int i1, int i2) const
{
	if (i1 > i2) return Bitset(0);
	if (badIndex(i1)) return Bitset(0);
	if (badIndex(i2)) return Bitset(0);
	
	Bitset b(i2-i1);
	for (int i = i1; i < i2; i++)
	{
		if ((*this)[i]) b.set(i-i1);
	}
	return b;
}

int Bitset::getLength() const { return length; }

std::string Bitset::asString() const
{
	std::string str;
	for (int i = 0; i < length; i++)
	{
		str = ((at(i)) ? '1' : '0') + str;
	}
	return str;
}

std::vector<uint8_t> Bitset::getDataVector() const { return data; }

std::string Bitset::asASCIIString() const { return std::string(data.begin(), data.end()); }

std::string Bitset::asHexString() const 
{ 
	std::stringstream ss;
	for (uint8_t val : data)
	{
		ss << std::setw(2) << std::setfill('0') << std::hex << (int)val;
	}
	return ss.str();
}