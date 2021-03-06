#include "Bitset.hpp"

using LiFiData::Bitset;

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

Bitset::Bitset() : Bitset(0) { }

Bitset::Bitset(const String& s) : length(s.length())
{
	for (int i = 0; i < length; i++)
	{
		if (s[length-i-1] == '1') set(i);
	}
}

Bitset::Bitset(const int* arr, const int l) : length(l*8)
{
	for (int i = 0; i < l; i++) data[i] = arr[i];
}

bool Bitset::badIndex(int i) const { return i < 0 || i >= MAX_BITSET_LENGTH; }

Bitset::Bitset(int l) : length(l)
{ 
	for (int i = 0; i < MAX_ARR_LENGTH; i++) data[i] = 0;
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

void Bitset::setLength(int l) { length = l; }

int Bitset::getLength() const { return length; }

String Bitset::asString() const
{
	String str;
	for (int i = 0; i < length; i++)
	{
		str += ((at(i)) ? '1' : '0');
	}
	return str;
}

String Bitset::asASCIIString() const
{
	String str;
	for (int i = 0; i < length; i += 8)
	{
		char letter = 0;
		for (int j = 0; j < 8; j++)
		{
			if (at(i+j)) letter |= 0x80 >> j;
		}
		str += letter;
	}
	return str;
}
