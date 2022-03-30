//Header file for compression information
#ifndef LIFICOMPRESSION
#define LIFICOMPRESSION

#include "Bitset.hpp"
#include "LiFiConstants.hpp"

#include <string>
#include <map>
#include <numeric>

using LiFiData::Bitset;
using LiFiData::stringToBitset;

namespace LiFiCompression
{	
	std::map<char, double> readSource(std::string source);
	
	LiFiData::Bitset writeLookupTableAsBitset(const codeTable&);
}

#endif