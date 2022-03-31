#include "LiFiCompression.hpp"

#include <vector>
#include <iostream>

using LiFiData::Bitset;

using LiFiCompression::freqTable;

Bitset LiFiCompression::writeLookupTableAsBitset(const codeTable& table)
{
	Bitset allData(0);
	for (char c : codeAlphabet)
	{
		std::vector<uint8_t> length;
		length.push_back(table.at(c).getLength());
		allData = allData + Bitset(length,4);
		allData = allData + table.at(c);
	}
	return allData;
}

const freqTable LiFiCompression::getFreqTable(const std::string& source)
{
	freqTable counts;
	for (char c : codeAlphabet) counts[c] = 0;
	for (char c : source) counts[c]++;
	
	const std::size_t result = std::accumulate(std::begin(counts), std::end(counts), 0,
									  [](const std::size_t previous, const std::pair<const char, const double>& p)
									  { return previous + p.second; });

	for (char c : codeAlphabet) counts[c] /= result;
	return counts;
}