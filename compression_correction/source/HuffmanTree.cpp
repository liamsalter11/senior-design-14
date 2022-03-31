#include "HuffmanTree.hpp"

#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

using LiFiCompression::HuffmanTree;
using LiFiCompression::HuffmanTreeNode;
using LiFiCompression::freqTable;
using LiFiCompression::codeAlphabet;
using LiFiData::Bitset;

HuffmanTree::HuffmanTree() { root = nullptr; alphabet = ""; refs = new unsigned int(1); }

HuffmanTree::HuffmanTree(const HuffmanTree& copy)
{
	root = copy.root;
	refs = copy.refs;
	(*refs)++;
}

HuffmanTree& HuffmanTree::operator=(const HuffmanTree& rhs)
{
	root = rhs.root;
	refs = rhs.refs;
	(*refs)++;
	return *this;
}

void addChildren(std::vector<HuffmanTreeNode*>& allNodes, int i, int& found)
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
}

void searchForNodes(std::vector<HuffmanTreeNode*>& allNodes, int& newNodes)
{
	int found = 0;
	int curNodes = allNodes.size();
	for (int i = curNodes-newNodes; i < curNodes; i++) addChildren(allNodes, i, found);
	newNodes = found;
}

std::vector<HuffmanTreeNode*> findAllNodes(HuffmanTreeNode* root)
{
	std::vector<HuffmanTreeNode*> allNodes;
	allNodes.push_back(root);
	int newNodes = 1;
	while (newNodes)
	{
		searchForNodes(allNodes, newNodes);
	}
	return allNodes;
}

void destroyTree(HuffmanTreeNode* root)
{
	if (!root) return;
	std::vector<HuffmanTreeNode*> allNodes = findAllNodes(root);
	for (auto& node : allNodes) delete node;
}

HuffmanTree::~HuffmanTree()
{
	(*refs)--;
	if (refs==0)
	{
		destroyTree(root);
		delete refs;
	}
}

std::vector<HuffmanTreeNode*> makeSymbolNodes(const freqTable& freqs)
{
	std::vector<HuffmanTreeNode*> nodes;
	
	//Create nodes for all symbols
	for (const auto& val : freqs)
	{
		HuffmanTreeNode* node = new HuffmanTreeNode;
		node->symbol = val.first;
		node->freq = val.second;
		nodes.push_back(node);
	}
	
	return nodes;
}

const auto nodeCompFunction = [](const HuffmanTreeNode* n1, const HuffmanTreeNode* n2){ return n1->freq<n2->freq; };

void sortByFreq(std::vector<HuffmanTreeNode*>& nodes)
{
	std::sort(nodes.begin(), nodes.end(), nodeCompFunction);
}

HuffmanTreeNode* makeParentNode(HuffmanTreeNode* l, HuffmanTreeNode* r)
{
	HuffmanTreeNode* parent = new HuffmanTreeNode;
	parent->left = l;
	parent->right = r;
	parent->freq = l->freq + r->freq;
	return parent;
}

void replaceChildrenWithParent(std::vector<HuffmanTreeNode*>& nodes, HuffmanTreeNode* parent)
{
	nodes.erase(nodes.begin(), nodes.begin()+2);
	nodes.insert(std::upper_bound(nodes.begin(), nodes.end(), parent, nodeCompFunction), parent);
}

void buildTreeFromNodes(std::vector<HuffmanTreeNode*>& nodes)
{
	while (nodes.size() > 1)
	{
		HuffmanTreeNode* parent = makeParentNode(nodes[0], nodes[1]);
		replaceChildrenWithParent(nodes, parent);
	}
}

void HuffmanTree::build(const freqTable& freqs)
{
	std::vector<HuffmanTreeNode*> nodes = makeSymbolNodes(freqs);
	std::sort(nodes.begin(), nodes.end(), nodeCompFunction);
	std::cout << "Building Tree\n";
	buildTreeFromNodes(nodes);
	root = nodes[0];
}

void searchFromParentNode(std::vector<std::pair<HuffmanTreeNode*, std::string>>& nodes)
{
	if (nodes[0].first->left) nodes.push_back(std::pair<HuffmanTreeNode*, std::string>(nodes[0].first->left, nodes[0].second+'0'));
	if (nodes[0].first->right) nodes.push_back(std::pair<HuffmanTreeNode*, std::string>(nodes[0].first->right, nodes[0].second+'1'));
	nodes.erase(nodes.begin());
}

Bitset checkLeafNode(std::vector<std::pair<HuffmanTreeNode*, std::string>>& nodes, char c)
{
	if (nodes[0].first->symbol == c) return Bitset(nodes[0].second);
	nodes.erase(nodes.begin());
	return Bitset(0);
}

Bitset checkNode(std::vector<std::pair<HuffmanTreeNode*, std::string>>& nodes, char c)
{
	if (!nodes[0].first->left && !nodes[0].first->right) return checkLeafNode(nodes, c);
	searchFromParentNode(nodes);
	return Bitset(0);
}

LiFiData::Bitset HuffmanTree::findCode(const char& c) const
{
	std::vector<std::pair<HuffmanTreeNode*, std::string>> nodes;
	nodes.push_back(std::pair<HuffmanTreeNode*, std::string>(root, ""));
	
	//Breadth first search for our symbol
	while (nodes.size() > 0)
	{
		Bitset check = checkNode(nodes, c);
		if (check.getLength()) return check;
	}
	return Bitset();
}

std::map<char, LiFiData::Bitset> HuffmanTree::getLookupTable() const
{
	std::map<char, LiFiData::Bitset> table;
	for (char c : codeAlphabet) { table[c] = findCode(c); }
	return table;
}