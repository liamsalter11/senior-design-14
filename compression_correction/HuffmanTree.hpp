#ifndef HUFFMANTREE
#define HUFFMANTREE

#include <map>
#include <utility>
#include <memory>

#include "Bitset.hpp"

namespace LiFiCompression
{
	class HuffmanTreeBuilder;
	class HuffmanTree;
	
	struct HuffmanTreeNode
	{
		//LiFiData::Bitset code;
		char symbol;
		double freq;
		HuffmanTreeNode* left = nullptr, *right = nullptr;
	};
};

class LiFiCompression::HuffmanTree
{
	private:
		HuffmanTreeNode* root;
		std::string alphabet;
		unsigned int* refs;
		
	public:
		HuffmanTree();
		HuffmanTree(const HuffmanTree&);
		HuffmanTree& operator=(const HuffmanTree&);
		~HuffmanTree();
		
		void build(const std::map<char, double>&);
		
		LiFiData::Bitset findCode(const char&) const;
		std::map<char, LiFiData::Bitset> getLookupTable() const;
		
		LiFiData::Bitset getBitset() const;
};

#endif