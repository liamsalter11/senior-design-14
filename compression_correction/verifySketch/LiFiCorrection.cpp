#include "LiFiCorrection.hpp"	

using LiFiData::Bitset;

namespace StateMachine
{
	const int prev1[4] = {0,2,0,2};
	const int prev2[4] = {1,3,1,3};
	const int in1[4] = {0,2,3,1};
	const int in2[4] = {3,1,0,2};
	const int out1[4] = {0,0,1,1};
	const int out2[4] = {0,0,1,1};
};

using StateMachine::prev1;
using StateMachine::prev2;
using StateMachine::in1;
using StateMachine::in2;
using StateMachine::out1;
using StateMachine::out2;

struct trellis
{
	int* metrics[4];
	int length;
	
	trellis(const int& l) : length(l)
	{
		for (int i=0;i<4;i++) metrics[i] = new int[length+1]();
		metrics[0][1] = metrics[0][2] = metrics[0][3] = 1000;
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

void calculatePathMetrics(trellis& trell, const Bitset& encoded)
{
	for (int i = 1; i < encoded.getLength()/2+1; i++)
	{
		int input = encoded[2*i-1] + (encoded[2*i-2] << 1);		
		for (int j = 0; j < 4; j++)
		{
			int met1 = trell[prev1[j]][i-1] + hamming(input, in1[j]);
			int met2 = trell[prev2[j]][i-1] + hamming(input, in2[j]);
			if (met1 < met2)
				trell[j][i] = met1;
			else
				trell[j][i] = met2;
		}
	}
}

Bitset findBestPath(trellis& trell)
{
	Bitset data(trell.length);
	int minState = -1, min = 30000;
	for (int i = 0; i < 4; i++)
	{
		if (trell[i][trell.length] < min)
		{
			min = trell[i][trell.length];
			minState = i;
		}
	}
	for (int i = trell.length-1; i>=0; i--)
	{
		if (trell[prev1[minState]][i] < trell[prev2[minState]][i])
		{
			if (out1[minState]) data.set(i);
			minState = prev1[minState];
		}
		else
		{
			if (out2[minState]) data.set(i);
			minState = prev2[minState];
		}
	}
	return data;
}

Bitset LiFiCorrection::decodeErrors(const Bitset& encoded)
{
	trellis trell(encoded.getLength()/2);
	calculatePathMetrics(trell, encoded);
	return findBestPath(trell);
}
