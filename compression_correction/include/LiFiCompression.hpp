//Header file for compression information
#ifndef LIFICOMPRESSION
#define LIFICOMPRESSION

#include "Bitset.hpp"

#include <string>
#include <map>
#include <numeric>

using LiFiData::Bitset;
using LiFiData::stringToBitset;

namespace LiFiCompression
{
	const std::string codeAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,'\"?!;: \n\t[]()";
	
	typedef std::map<char, LiFiData::Bitset> codeTable;	
	
	//Wrapper struct for frequency map
	struct freqTable
	{
		private:
			std::map<char, double> table;
		
		public:
			freqTable()
			{
				for (char c : codeAlphabet) table[c] = 0;
			}
			double& operator[](char c) { return table[c]; }
			
			std::map<char, double>::iterator begin() { return table.begin(); }
			std::map<char, double>::iterator end() { return table.end(); }
			const std::map<char, double>::const_iterator begin() const { return table.begin(); }
			const std::map<char, double>::const_iterator end() const { return table.end(); }
	};
	
	typedef std::map<char, freqTable> firstOrderFreqTable;
	
	const freqTable getFreqTable(const std::string&);
	const firstOrderFreqTable getFirstOrderFreqTable(const std::string&);
	
	LiFiData::Bitset writeLookupTableAsBitset(const codeTable&);
}

#endif