#include "LiFiSimulation.hpp"

#include <iostream>

using LiFiSimulation::setCompressionTestStringVector;
using LiFiSimulation::runFullCompressionTest;

using LiFiSimulation::setLengths;
using LiFiSimulation::setErrorRates;
using LiFiSimulation::setNumberofRounds;
using LiFiSimulation::runErrorRateSimulation;

using LiFiSimulation::compressAndSend;

int main()
{
	std::vector<std::string> strings({"this is an end to end test", 
										"here is a string to send across the whole system",
										"hopefully this string does not have any weird errors"});
	setCompressionTestStringVector(strings);
	runFullCompressionTest();
	
	setLengths(100, 500, 50);
	setErrorRates(0, 0.01, 0.001);
	setNumberofRounds(100);
	runErrorRateSimulation(1000);
	
	for (std::string s : strings) compressAndSend(s, 0);
	return 0;
}