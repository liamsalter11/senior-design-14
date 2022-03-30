#ifndef LIFISIMULATION
#define LIFISIMULATION

#include <string>
#include <vector>
#include <utility>

namespace LiFiSimulation
{
	//Error Correction Simulation
	void setLengths(const unsigned int&, const unsigned int&, const unsigned int&);
	void setErrorRates(const double&, const double&, const double&);
	void setNumberofRounds(const unsigned int&);
	
	std::vector<double> getErrorRateVector();
	std::vector<unsigned int> getFrameLengthVector();
	unsigned int getNumberofRounds();
	
	std::vector<double> runErrorRateSimulation(const unsigned int&);
	std::vector<double> runLengthSimulation(const double&);
	
	//Compression Simuation
	void setCompressionTestString(const std::string&);
	void setCompressionTestStringVector(const std::vector<std::string>&);
	
	std::string getCompressionTestString();
	std::vector<std::string> getCompressionTestStringVector();
	
	std::pair<std::string, double> runSingleCompressionTest();
	std::vector<std::pair<std::string, double>> runFullCompressionTest();
	
	//Full Simulation
	void compressAndSend(const std::string&, const double&);
};

#endif