#include <fstream>
#include <sstream>
#include <iostream>

#include "HuffmanTree.hpp"
#include "LiFiCompression.hpp"
#include "LiFiCodeAnalysis.hpp"

using LiFiCompression::readSource;

using LiFiCompression::HuffmanTree;
using LiFiCompression::HuffmanTreeNode;

int main()
{
	std::cout << "Starting\n";
	std::ifstream dataFile("data/allcut.txt", std::ifstream::in);
	std::stringstream sourceText;
	sourceText << dataFile.rdbuf();
	
	std::cout << "Reading books\n";
	std::map<char, double> sourceData =  readSource(sourceText.str());
	std::cout << "Building Huffman Tree\n";
	HuffmanTree tree;
	tree.build(sourceData);
	tree.findCode('a');
	
	std::map<char, std::string> stringCodes;
	for (auto data : sourceData)
	{
		std::cout << data.first << ": " << tree.findCode(data.first).asString() << '\n';
		stringCodes[data.first] = tree.findCode(data.first).asString();
	}
	
	LiFiCodeAnalysis::findSyncCode(stringCodes);
	return 0;
}