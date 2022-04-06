#include "LiFiCompression.hpp"

#include <vector>
#include <iostream>

using LiFiData::Bitset;

using LiFiCompression::freqTable;
using LiFiCompression::firstOrderFreqTable;
using LiFiCompression::codeTable;
using LiFiCompression::firstOrderCodeTable;
using LiFiCompression::codeAlphabet;

freqTable convertCountTableToFreqTable(freqTable& countTable)
{
	freqTable fTable;
	const std::size_t total = std::accumulate(countTable.begin(), countTable.end(), 0,
									  [](const std::size_t previous, const std::pair<const char, const double>& p)
									  { return previous + p.second; });
									  
	for (char c : codeAlphabet) fTable[c] = countTable[c] / total;
	return fTable;
}

Bitset LiFiCompression::writeLookupTableAsBitset(const codeTable& table)
{
	Bitset allData(0);
	for (char c : codeAlphabet)
	{
		int codeLength = table.at(c).getLength();
		std::vector<uint8_t> length(1, codeLength);
		allData = allData + Bitset(length,4);
		allData = allData + table.at(c);
	}
	return allData;
}

const freqTable LiFiCompression::getFreqTable(const std::string& source)
{
	freqTable counts;
	for (char c : source) counts[c]++;
	
	return convertCountTableToFreqTable(counts);
}

const firstOrderFreqTable getFirstOrderFreqTable(const std::string& source)
{
	char lastSymbol = 'a';
	firstOrderFreqTable table;
	
	for (char c : source)
	{
		table[lastSymbol][c]++;
		lastSymbol = c;
	}
	
	for (char c : codeAlphabet) table[c] = convertCountTableToFreqTable(table[c]);
	
	return table;
}

const codeTable makeCodeTable(const freqTable&)
{
	return codeTable();
}

const firstOrderCodeTable makeFirstOrderCodeTable(const firstOrderFreqTable&)
{
	return firstOrderCodeTable();
}