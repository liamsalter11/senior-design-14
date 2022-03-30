#include "LiFiCorrection.hpp"	
	
#include <iostream>

using LiFiData::Bitset;

unsigned int codeLength;
double channelErrorRate;

struct statemachine
{
	const int prev1[4] = {0,2,0,2};
	const int prev2[4] = {1,3,1,3};
	const int in1[4] = {0,2,3,1};
	const int in2[4] = {3,1,0,2};
	const int out1[4] = {0,0,1,1};
	const int out2[4] = {0,0,1,1};
};

struct trellis
{
	int* metrics[4];
	
	trellis(const int& length)
	{
		for (int i=0;i<4;i++)
		{
			metrics[i] = new int[length+1];
			for (int j=0;j<length+1;j++) metrics[i][j] = 0;
		}
		
	}
	
	~trellis()
	{
		for (int i=0;i<4;i++) delete [] metrics[i];
	}
	
	int* operator[](const int& i) { return metrics[i]; }
};

int hamming(int a, int b)
{
	int result = 0;
	for (int i = 0; i < 64; i++)
	{
		result += ((a&0x1) != (b&0x1));
		a = a >> 1;
		b = b >> 1;
	}
	return result;
}
	
Bitset LiFiCorrection::convolve(const Bitset& data)
{
	Bitset convolved(data.getLength()*2);
	bool a = 0, b = 0, c = 0;
	for (int i = 0; i < data.getLength(); i++)
	{
		a = b;
		b = c;
		c = data[i];
		if (a^b^c) convolved.set(2*i);
		if (a^c) convolved.set(2*i + 1);
	}
	return convolved;
}

Bitset LiFiCorrection::decodeErrors(const Bitset& encoded)
{
	Bitset data(encoded.getLength()/2);
	
	statemachine machine;
	trellis trell(data.getLength());
	trell[0][1] = trell[0][2] = trell[0][3] = 1000;
	
	//Calculate path metrics
	for (int i = 1; i < data.getLength()+1; i++)
	{
		int input = encoded[2*i-1] + (encoded[2*i-2] << 1);		
		for (int j = 0; j < 4; j++)
		{
			int met1 = trell[machine.prev1[j]][i-1] + hamming(input, machine.in1[j]);
			int met2 = trell[machine.prev2[j]][i-1] + hamming(input, machine.in2[j]);
			if (met1 < met2)
				trell[j][i] = met1;
			else
				trell[j][i] = met2;
		}
	}
	
	int min = 100000;
	int minState = -1;
	for (int i = 0; i < 4; i++)
	{
		if (trell[i][data.getLength()] < min)
		{
			min = trell[i][data.getLength()];
			minState = i;
		}
	}
	for (int i = data.getLength()-1; i>=0; i--)
	{
		if (trell[machine.prev1[minState]][i] < trell[machine.prev2[minState]][i])
		{
			if (machine.out1[minState]) data.set(i);
			minState = machine.prev1[minState];
		}
		else
		{
			if (machine.out2[minState]) data.set(i);
			minState = machine.prev2[minState];
		}
	}
	return data;
}
