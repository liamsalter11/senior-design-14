#include "HuffmanTree.hpp"

#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

using LiFiCompression::HuffmanTree;
using LiFiCompression::HuffmanTreeNode;
using LiFiData::Bitset;

HuffmanTree::HuffmanTree() { root = nullptr; alphabet = ""; refs = new unsigned int(1); }

HuffmanTree(const HuffmanTree& copy)
{
	root = copy.root;
	refs = copy.refs;
	(*refs)++;
}

HuffmanTree& operator=(const HuffmanTree& rhs)
{
	root = rhs.root;
	refs = rhs.refs;
	(*refs)++;
	return *this;
}

HuffmanTree::~HuffmanTree()
{
	(*refs)--;
	if (refs==0)
	{
		if (!root) return;
		std::vector<HuffmanTreeNode*> allNodes;
		allNodes.push_back(root);
		unsigned int newNodes = 1;
		while (newNodes)
		{
			int found = 0;
			int curNodes = allNodes.size();
			for (int i = curNodes-newNodes; i < curNodes; i++)
			{
				if (allNodes[i]->left)
				{
					allNodes.push_back(allNodes[i]->left);
					found++;
				}
				if (allNodes[i]->right)
				{
					allNodes.push_back(allNodes[i]->right);
					found++;
				}
				newNodes--;
			}
			newNodes = found;
		}
		
		for (auto& node : allNodes) delete node;
		delete refs;
	}
}

void HuffmanTree::build(const std::map<char, double>& freqs)
{
	std::vector<HuffmanTreeNode*> nodes;
	
	//Create nodes for all symbols
	for (const auto& val : freqs)
	{
		HuffmanTreeNode* node = new HuffmanTreeNode;
		node->symbol = val.first;
		node->freq = val.second;
		nodes.push_back(node);
		
		alphabet += val.first;
	}
	
	//Sort nodes by freq
	auto nodeCompFunction = [](const HuffmanTreeNode* n1, const HuffmanTreeNode* n2){ return n1->freq<n2->freq; };
	std::sort(nodes.begin(), nodes.end(), nodeCompFunction);
	
	std::cout << "Building Tree\n";
	//Build tree
	while (nodes.size() > 1)
	{
		HuffmanTreeNode* parent = new HuffmanTreeNode;
		parent->left = nodes[0];
		parent->right = nodes[1];
		parent->freq = nodes[0]->freq + nodes[1]->freq;
		nodes.erase(nodes.begin(), nodes.begin()+2);
		nodes.insert(std::upper_bound(nodes.begin(), nodes.end(), parent, nodeCompFunction), parent);
	}
	root = nodes[0];
}

LiFiData::Bitset HuffmanTree::findCode(const char& c) const
{
	std::vector<std::pair<HuffmanTreeNode*, std::string>> nodes;
	nodes.push_back(std::pair<HuffmanTreeNode*, std::string>(root, ""));
	
	//Breadth first search for our symbol
	while (nodes.size() > 0)
	{
		auto nodeCode = nodes[0];
		
		//Leaf node
		if (!nodeCode.first->left && !nodeCode.first->right)
		{
			if (nodeCode.first->symbol == c)
			{
				return Bitset(nodeCode.second);
			}
			else
			{
				nodes.erase(nodes.begin());
			}
		}
		else
		{
			//Parent node
			if (nodeCode.first->left)
			{
				nodes.push_back(std::pair<HuffmanTreeNode*, std::string>(nodeCode.first->left, nodeCode.second+'0'));
			}
			if (nodeCode.first->right)
			{
				nodes.push_back(std::pair<HuffmanTreeNode*, std::string>(nodeCode.first->right, nodeCode.second+'1'));
			}
			nodes.erase(nodes.begin());
		}
	}
	return Bitset();
}

std::map<char, LiFiData::Bitset> HuffmanTree::getLookupTable() const
{
	std::map<char, LiFiData::Bitset> table;
	for (char c : alphabet) { table[c] = findCode(c); }
	return table;
}