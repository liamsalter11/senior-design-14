#include "LiFiCodeAnalysis.hpp"

#include <vector>
#include <algorithm>

using LiFiCompression::HuffmanTree;

void LiFiCodeAnalysis::findSyncCode(const HuffmanTree& tree)
{
	
}

void LiFiCodeAnalysis::quantize(const HuffmanTree& tree)
{
	auto codes = tree.getLookupTable();
	
	std::vector<Bitset> codeVector;
	for (auto &val : codes) codeVector.push_back(val.second);
	
	std::sort(codeVector.begin(), codeVector.end(), [](const Bitset& b1, const Bitset& b2){ return b1.getLength()<b2.getLength() });
	
	int largestLength = codeVector.back().getLength();
	int codeLengths[largestLength] = { 0 };
	
	for (auto& code : codeVector) codeLengths[code.getLength()-1]++;
	
	std::cout << "Quantized Huffman Tree:\n";
	for (int i = 0; i < largestLength; i++) std::cout << i+1 << ": " << codeLengths[i] << '\n';
}