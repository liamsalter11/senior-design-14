#include "LiFiSimulation.hpp"
#include "LiFiCorrection.hpp"
#include "LiFiCompression.hpp"
#include "Bitset.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <iomanip>

/***Error Correction Simulations***/

using LiFiCorrection::convolve;
using LiFiCorrection::decodeErrors;
using LiFiData::Bitset;
using LiFiData::hammingDistance;

std::vector<double> errorRates, finalErrors;
std::vector<unsigned int> lengths;
unsigned int numRounds;

void LiFiSimulation::setLengths(const unsigned int& lengthLow, const unsigned int& lengthHigh, const unsigned int& lengthStep)
{
	lengths = std::vector<unsigned int>();
	for (unsigned int i = lengthLow; i <= lengthHigh; i += lengthStep) lengths.push_back(i);
}

void LiFiSimulation::setErrorRates(const double& errLow, const double& errHigh, const double& errStep)
{
	errorRates = std::vector<double>();
	
	for (double i = errLow; i <= errHigh; i += errStep) errorRates.push_back(i);	
}

void LiFiSimulation::setNumberofRounds(const unsigned int& r) { numRounds = r; }

std::vector<double> LiFiSimulation::getErrorRateVector() { return errorRates; }

std::vector<unsigned int> LiFiSimulation::getFrameLengthVector() { return lengths; }

unsigned int LiFiSimulation::getNumberofRounds() { return numRounds; }

Bitset makeErrors(const Bitset& data, const double& errRate)
{
	Bitset corrupted = data;
	
	for (int i = 0; i < data.getLength(); i++)
	{
		double test = (double)rand() / RAND_MAX;
		if (test < errRate) corrupted.toggle(i); 
	}
	return corrupted;
}

void runSim(const unsigned int& length, const double& errorRate)
{
	std::vector<int> errorCounts;
	for (int i = 0; i < numRounds; i++)
	{
		//Create bitstream
		Bitset rxData(length);
		for (int i = 0; i < length; i++)
		{
			if (rand() % 2) rxData.set(i);
		}
		
		//Simulate
		Bitset convolved = convolve(rxData);
		
		Bitset corrupted = makeErrors(convolved, errorRate);
		Bitset decoded = decodeErrors(corrupted);
		
		//Count Errors
		int numErrs = hammingDistance(rxData, decoded);
		errorCounts.push_back(numErrs);
	}
	double avg = 0;
	for (int e : errorCounts) avg += e;
	avg /= numRounds;
	
	std::cout << std::scientific << std::setprecision(4) << avg/length << " for length " 
			  << std::setw(5) << length << " and error " 
			  << std::setprecision(2) << errorRate << '\n';
	finalErrors.push_back(avg / length);
}

std::vector<double> LiFiSimulation::runErrorRateSimulation(const unsigned int& length)
{
	std::cout << "Starting simulation...\n";
	finalErrors = std::vector<double>();
	srand(time(NULL));
	
	for (double e : errorRates)
	{
		runSim(length, e);
	}
	return finalErrors;
}

std::vector<double> LiFiSimulation::runLengthSimulation(const double& errorRate)
{
	std::cout << "Starting simulation...\n";
	finalErrors = std::vector<double>();
	srand(time(NULL));
	
	for (unsigned int l : lengths)
	{
		runSim(l, errorRate);
	}
	return finalErrors;
}

/***Compression Simulations***/

using LiFiCompression::compress;
using LiFiCompression::decodeSymbols;

std::vector<std::string> testStringVector;
std::string testString;

void LiFiSimulation::setCompressionTestString(const std::string& s) { testString = s; }

void LiFiSimulation::setCompressionTestStringVector(const std::vector<std::string>& s) { testStringVector = s; }

std::string LiFiSimulation::getCompressionTestString() { return testString; }

std::vector<std::string> LiFiSimulation::getCompressionTestStringVector() { return testStringVector; }

std::pair<std::string, double> runCompressionTest(const std::string &s)
{
	std::cout << "Uncompressed: " << s << '\n';
	Bitset compressed = compress(s);
	std::cout << "Decoded: " << decodeSymbols(compressed) << '\n';

	double ratio = (double) compressed.getLength() / (s.size()*8);
	std::cout << "Ratio: " << ratio << '\n';
	return std::pair<std::string, double>(compressed.asString(), ratio);
}

std::pair<std::string, double> LiFiSimulation::runSingleCompressionTest()
{
	std::cout << "Running Compression Test...\n";
	return runCompressionTest(testString);
}

std::vector<std::pair<std::string, double>> LiFiSimulation::runFullCompressionTest()
{
	std::vector<std::pair<std::string, double>> results;
	for (std::string s : testStringVector)
	{
		results.push_back(runCompressionTest(s));
	}
	return results;
}

void LiFiSimulation::compressAndSend(const std::string& data, const double& errRate)
{
	std::cout << "End to end simulation for string: " << data << '\n';
	Bitset compressed = compress(data);
	std::string check = decodeSymbols(compressed);
	Bitset txData = convolve(compressed);
	Bitset channelData = makeErrors(txData, errRate);
	Bitset rxData = decodeErrors(txData);
	std::string result = decodeSymbols(rxData);
	std::cout << "Decoded                         : " << result << '\n';
	
}