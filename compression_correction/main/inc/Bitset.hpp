#ifndef BITSET
#define BITSET

#include <vector>
#include <cstdint>
#include <string>

namespace LiFiData
{
	class Bitset;
	
	int hammingDistance(const Bitset&, const Bitset&);
	
	enum stringType
	{
		BINARY_STRING,
		HEX_STRING,
		ASCII_STRING
	};
};

class LiFiData::Bitset
{
	private:
		int length;
		std::vector<uint8_t> data;
		
		void fromBinaryString(const std::string&);
		void fromHexString(const std::string&);
		void fromASCIIString(const std::string&);
		
		bool badIndex(int) const;
		
	public:
		Bitset();
		Bitset(int);
		Bitset(const std::string&, stringType=BINARY_STRING);
		Bitset(const std::vector<uint8_t>&);
		Bitset(const std::vector<uint8_t>&, int);
		
		const Bitset operator+(const Bitset&) const;
		
		bool operator[](int) const;
		bool at(int) const;
		void set(int);
		void toggle(int);
		void clear(int);
		
		Bitset getSubset(int, int) const;
		int getLength() const;
		std::string asString() const;
		std::string asASCIIString() const;
		std::string asHexString() const;
		std::vector<uint8_t> getDataVector() const;
		
		friend int LiFiData::hammingDistance(const Bitset&, const Bitset&);
};

#endif