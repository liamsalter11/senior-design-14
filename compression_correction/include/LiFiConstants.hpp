#ifndef LIFICONSTS
#define LIFICONSTS

#include <map>

typedef std::map<char, LiFiData::Bitset> codeTable;

namespace LiFiConstants
{
	const std::string codeAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,'\"?!;: \n\t[]()";
	const std::string codeData = "07db83cdc1fa6075b03c581ccc0f0e068c016d4019900684016e8019a0072f03d80032000d8083ffc1efe07bf039781bf4069d416bd018d4068141695018e40699416a9018c406914168801c3c0f1e07f783dbc1f5e072703e381d1c0f66069a816aa018c8069281682018f0069c816ec01a1806b681633018b806a3c164e019f806d5c1fe6067601998";
	
	
}

#endif