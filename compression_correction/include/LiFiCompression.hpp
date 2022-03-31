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
	typedef std::map<char, LiFiData::Bitset> codeTable;
	typedef std::map<char, double> freqTable;
	typedef std::map<char, std::map<char, double>> firstOrderFreqTable;	
	
	const std::string codeAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,'\"?!;: \n\t[]()";
	
	const freqTable getFreqTable(const std::string&);
	const firstOrderFreqTable getFirstOrderFreqTable(const std::string&);
	
	LiFiData::Bitset writeLookupTableAsBitset(const codeTable&);
}

#endif