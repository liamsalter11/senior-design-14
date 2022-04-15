#ifndef BITSET
#define BITSET

#include <stdint.h>
#include <Arduino.h>

namespace LiFiData
{
	class Bitset;
	
	int hammingDistance(const Bitset&, const Bitset&);
};

class LiFiData::Bitset
{
	private:
		const static int MAX_LENGTH = 128; 
	
		int length;
		uint8_t data[MAX_LENGTH];
		
		bool badIndex(int) const;
		
	public:
		Bitset();
		Bitset(int);
		Bitset(const String&);
		
		const Bitset operator+(const Bitset&) const;
		
		bool operator[](int) const;
		bool at(int) const;
		void set(int);
		void toggle(int);
		void clear(int);
		
		Bitset getSubset(int, int) const;
		int getLength() const;
		String asString() const;
		
		friend int LiFiData::hammingDistance(const Bitset&, const Bitset&);
};

#endif
