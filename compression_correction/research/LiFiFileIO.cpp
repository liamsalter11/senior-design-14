#include "LiFiFileIO.hpp"

#include <iterator>

#include "LiFiCompression.hpp"

using LiFiFileIO::ReadFile;
using LiFiFileIO::WriteFile;

using LiFiData::Bitset;

ReadFile::ReadFile(const std::string& filename) : file(filename, std::ios::binary) { }

Bitset ReadFile::readFileAsBitset()
{
	if (file.is_open())
	{
		std::vector<uint8_t> rawData(std::istreambuf_iterator<char>(file), {});
		return Bitset(rawData);
	}
	return Bitset();
}

std::map<char, Bitset> ReadFile::readFileAsLookupTable()
{
	std::map<char, Bitset> table;
	
	std::vector<uint8_t> rawData;
	if (file.is_open())
	{
		rawData = std::vector<uint8_t>(std::istreambuf_iterator<char>(file), {});
	}
	else return table;
	
	Bitset allData(rawData);
	unsigned int position = 0;
	for (char c : LiFiCompression::alphabet)
	{
		unsigned int codeLength = allData.getSubset(position, position+8).getDataVector()[0];
		position += 8;
		Bitset code = allData.getSubset(position, position+codeLength);
		position += codeLength;
		table[c] = code;
	}
	return table;
}

void ReadFile::close()
{
	file.close();
}

WriteFile::WriteFile(const std::string& filename) : file(filename, std::ios::binary) { }

void WriteFile::writeBitset(const Bitset& data)
{
	std::vector<uint8_t> rawData = data.getDataVector();
	if (file.is_open())
	{
		for (uint8_t val : rawData) file << val;
	}
}

void WriteFile::writeLookupTable(const std::map<char, LiFiData::Bitset>& table)
{
	if (!file.is_open()) return;
	
	Bitset allData(0);
	for (char c : LiFiCompression::alphabet)
	{
		std::vector<uint8_t> length;
		length.push_back(table.at(c).getLength());
		allData = allData + Bitset(length);
		allData = allData + table.at(c);
	}
	writeBitset(allData);
}

void WriteFile::close()
{ 
	file.close(); 
}