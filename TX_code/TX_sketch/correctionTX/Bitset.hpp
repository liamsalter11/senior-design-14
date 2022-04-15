#ifndef BITSET
#define BITSET

#include <stdint.h>
#include <Arduino.h>

namespace LiFiData
{
	const int MAX_BITSET_LENGTH = 1024; 
	
	class Bitset;
	
	int hammingDistance(const Bitset&, const Bitset&);
};

class LiFiData::Bitset
{
	private:
		const static int MAX_ARR_LENGTH = LiFiData::MAX_BITSET_LENGTH/8;
		int length;
		uint8_t data[MAX_ARR_LENGTH];
		
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
