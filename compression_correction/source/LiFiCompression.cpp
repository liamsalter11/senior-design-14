#include "LiFiCompression.hpp"

#include <vector>

using LiFiData::Bitset;

Bitset LiFiCompression::writeLookupTableAsBitset(const codeTable& table)
{
	Bitset allData(0);
	for (char c : LiFiConstants::codeAlphabet)
	{
		std::vector<uint8_t> length;
		length.push_back(table.at(c).getLength());
		allData = allData + Bitset(length);
		allData = allData + table.at(c);
	}
	return allData;
}

std::map<char, double> LiFiCompression::readSource(std::string source)
{
	std::map<char, double> counts;
	for (char c : LiFiConstants::codeAlphabet) counts[c] = 0;
	for (char c : source) counts[c]++;
	const std::size_t result = std::accumulate(std::begin(counts), std::end(counts), 0,
									  [](const std::size_t previous, const std::pair<const char, const double>& p)
									  { return previous + p.second; });

	for (char c : LiFiConstants::codeAlphabet) counts[c] /= result;
	return counts;
}