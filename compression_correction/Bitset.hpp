#ifndef BITSET
#define BITSET

#include <vector>
#include <cstdint>
#include <string>

namespace LiFiData
{
	class Bitset;
	
	unsigned int hammingDistance(const Bitset&, const Bitset&);
	
	Bitset stringToBitset(const std::string&);
	Bitset intArrToBitset(const int*, const unsigned int);
};

class LiFiData::Bitset
{
	private:
		unsigned int length;
		std::vector<uint8_t> data;
		
	public:
		Bitset();
		Bitset(const unsigned int&);
		Bitset(const std::string&);
		Bitset(const std::vector<uint8_t>&);
		Bitset(const std::vector<uint8_t>&, const unsigned int&);
		
		const Bitset operator+(const Bitset&) const;
		
		bool operator[](const unsigned int&) const;
		bool at(const unsigned int&) const;
		void set(const unsigned int&);
		void toggle(const unsigned int&);
		void clear(const unsigned int&);
		
		Bitset getSubset(const unsigned int&, const unsigned int&) const;
		unsigned int getLength() const;
		std::string asString() const;
		std::vector<uint8_t> getDataVector() const;
		
		friend unsigned int LiFiData::hammingDistance(const Bitset&, const Bitset&);
};

#endif