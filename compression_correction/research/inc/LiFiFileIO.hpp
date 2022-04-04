#ifndef LIFIFILEIO
#define LIFIFILEIO

#include <string>
#include <fstream>
#include <map>

#include "Bitset.hpp"

namespace LiFiFileIO
{
	class ReadFile;
	class WriteFile;
};

class LiFiFileIO::ReadFile
{
	private:
		std::ifstream file;
		
	public:
		ReadFile(const std::string&);
	
		LiFiData::Bitset readFileAsBitset();
		std::map<char, LiFiData::Bitset> readFileAsLookupTable();
		
		void close();
};

class LiFiFileIO::WriteFile
{
	private:
		std::ofstream file;
		
	public:
		WriteFile(const std::string&);
		
		void writeBitset(const LiFiData::Bitset&);
		void writeLookupTable(const std::map<char, LiFiData::Bitset>&);
		
		void close();
};

#endif