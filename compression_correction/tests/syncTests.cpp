#include "LiFiCompressionOld.hpp"

#include <iostream>

using LiFiCompressionOld::compress;
using LiFiCompressionOld::decodeSymbols;

const std::string shortTestString = "test compression";
const std::string syncTestString = "This is a test to see how long it may take for my Huffman code to resynchronize";

void basicCompressionTest(const std::string& s)
{
	std::cout << "Testing accurate compression of: " << s << '\n';
	Bitset compressed = compress(s);
	std::cout << "Uncompressed Length: " << s.size()*8 << '\n'
			  << "Compressed Length  : " << compressed.getLength() << '\n'
			  << "Ratio              : " << (double) compressed.getLength()/(s.size()*8) << '\n';
	std::string check = decodeSymbols(compressed);
	if (s == check) std::cout << "Decoded correctly!\n\n";
	else std::cout << "Errors in decoding: " << check << "\n\n";
}

void desyncCompressionTest(const std::string& s)
{
	std::cout << "Testing resynchronization of: " << s << '\n'
			  << "Testing single bit error for the first 20 bits of transmission\n";

	Bitset compressed = compress(s);
	for (int i = 0; i < 20; i++)
	{
		compressed.toggle(i);
		std::string decoded = decodeSymbols(compressed);
		std::cout << "Result of bit error at position " << i << ":\n" << decoded << "\n\n";
		compressed.toggle(i);
	}
	
	std::cout << '\n';
}

int main()
{
	std::cout << "Huffman code synchronization tests\n"
			  << "Using v1.0 of LiFiCompression\n"
			  << "Pitt ECE Senior Design Team 14, Preston Brazzle\n\n";
			  
	basicCompressionTest("test compression");
	basicCompressionTest(syncTestString);
	
	desyncCompressionTest(shortTestString);
	desyncCompressionTest(syncTestString);
	
	return 0;
}