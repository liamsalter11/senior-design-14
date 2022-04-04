#include <fstream>
#include <sstream>
#include <iostream>

#include "HuffmanTree.hpp"
#include "LiFiCompression.hpp"
#include "Bitset.hpp"

using LiFiCompression::getFreqTable;
using LiFiCompression::freqTable;
using LiFiCompression::codeTable;

using LiFiCompression::HuffmanTree;
using LiFiCompression::HuffmanTreeNode;
using LiFiCompression::writeLookupTableAsBitset;

using LiFiData::Bitset;

int main()
{
	std::cout << "Starting\n";
	std::ifstream dataFile("../data/allcut.txt", std::ifstream::in);
	std::stringstream sourceText;
	sourceText << dataFile.rdbuf();
	std::cout << "source length: " << sourceText.str().size();
	
	std::cout << "Reading books\n";
	freqTable sourceData =  getFreqTable(sourceText.str());
	std::cout << "Building Huffman Tree\n";
	HuffmanTree tree;
	tree.build(sourceData);
	
	std::map<char, std::string> stringCodes;
	for (auto data : sourceData)
	{
		std::cout << data.first << ": " << tree.findCode(data.first).asString() << '\n';
		stringCodes[data.first] = tree.findCode(data.first).asString();
	}
	
	codeTable table = tree.getLookupTable();
	Bitset squished = writeLookupTableAsBitset(table);
	
	std::cout << "Tree String: " << squished.asHexString();
	return 0;
}