#include "Bitset.hpp"

#include <iostream>

using LiFiData::Bitset;

unsigned int LiFiData::hammingDistance(const Bitset& set1, const Bitset& set2)
{
	if (set1.length != set2.length) return 0;
	unsigned int dist = 0;
	for (int i = 0; i < set1.length; i++)
	{
		if (set1[i] != set2[i]) dist++;
	}
	return dist;
}

Bitset LiFiData::stringToBitset(const std::string& str)
{
	Bitset b(str.size());
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '1') b.set(i);
	}
	return b;
}

Bitset LiFiData::intArrToBitset(const int* arr, const unsigned int size)
{
	Bitset b(size);
	for (int i = 0; i < size; i++)
	{
		if (arr[i]) b.set(i);
	}
	return b;
}

Bitset::Bitset()
{
	length = 0;
}

Bitset::Bitset(const std::string& s) : Bitset(s.size())
{
	for (int i = 0; i < length; i++)
	{
		if (s[i] == '1') set(i);
	}
}

Bitset::Bitset(const std::vector<uint8_t>& raw) : data(raw), length(raw.size()*8) {}

Bitset::Bitset(const std::vector<uint8_t>& raw, const unsigned int& l) : data(raw), length(l) {}

Bitset::Bitset(const unsigned int& l) : length(l)
{
	for (int i = 0; i < l/8+1; i++) data.push_back(0);
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

bool Bitset::operator[](const unsigned int& i) const
{
	return at(i);
}

bool Bitset::at(const unsigned int& i) const
{
	return data[i/8] & (0x1<<(i%8));
}

void Bitset::set(const unsigned int& i)
{
	data[i/8] |= (0x1<<(i%8));
}

void Bitset::toggle(const unsigned int& i)
{
	data[i/8] ^= (0x1<<(i%8));
}

void Bitset::clear(const unsigned int& i)
{
	data[i/8] &= ~(0x1<<(i%8));
}

Bitset Bitset::getSubset(const unsigned int& i1, const unsigned int& i2) const
{
	Bitset b(i2-i1);
	for (int i = i1; i < i2; i++)
	{
		if ((*this)[i]) b.set(i-i1);
	}
	return b;
}

unsigned int Bitset::getLength() const { return length; }

std::string Bitset::asString() const
{
	std::string str;
	for (int i = 0; i < length; i++)
	{
		str += (at(i)) ? '1' : '0';
	}
	return str;
}

std::vector<uint8_t> Bitset::getDataVector() const { return data; }