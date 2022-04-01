#include "Bitset.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

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

Bitset(const std::string& s, stringType type)
{
	switch (type)
	{
		case BINARY:
			fromBinaryString(s);
			break;
		case HEX:
			fromHexString(s);
			break;
		case ASCII:
			fromASCIIString(s);
			break;
	}
}

Bitset::Bitset(const std::vector<uint8_t>& raw) : data(raw), length(raw.size()*8) {}

Bitset::Bitset(const std::vector<uint8_t>& raw, int l) : data(raw), length(l) {}

Bitset::Bitset(int l) : length(l)
{
	for (int i = 0; i < l/8+(l%8)?1:0; i++) data.push_back(0);
}

const Bitset Bitset::operator+(const Bitset& rhs) const
{
	//This could be a little better
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
	return data[i/8] & (0x1<<(i%8));
}

void Bitset::set(int i)
{
	data[i/8] |= (0x1<<(i%8));
}

void Bitset::toggle(int i)
{
	data[i/8] ^= (0x1<<(i%8));
}

void Bitset::clear(int i)
{
	data[i/8] &= ~(0x1<<(i%8));
}

Bitset Bitset::getSubset(int i1, int i2) const
{
	//This could probably be better as well
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
		str += (at(i)) ? '1' : '0';
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