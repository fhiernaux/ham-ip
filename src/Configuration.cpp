/*
 * Configuration.cpp
 *
 *  Created on: Jan 5, 2016
 *      Author: seb
 */

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "Configuration.h"

using namespace std;

Configuration::Configuration(int argc, char **argv, string filename) {
	initializeOptions();
	parseCommandLine(argc, argv);
	parseFile(filename);
	updateStore();
	checkArguments();
}

Configuration::~Configuration() {

}

void Configuration::initializeOptions(void) {
	optionDescription.add_options()
	            ("help", "Show help message")
	            ("callsign", po::value<string>(), "Station callsign")
				("interface_name", po::value<string>(), "TUN interface name")
				;
}

void Configuration::parseCommandLine(int argc, char** argv) {
	po::store(po::parse_command_line(argc, argv, optionDescription), programOptions);
}

void Configuration::showHelp(void) {
	cout << optionDescription << endl;
	exit(-1);
}

const char *Configuration::getCallsign(void) {
	return programOptions["callsign"].as<string>().c_str();
}

const char* Configuration::getInterfaceName(void) {
	return programOptions["interface_name"].as<string>().c_str();
}

void Configuration::parseFile(std::string fileName) {
	ifstream ifs(fileName.c_str());
	if (ifs.good()) {
		po::store(po::parse_config_file(ifs, optionDescription), programOptions);
	} else {
		cerr << "Warning: no configuration file found" << endl;
	}
}

void Configuration::updateStore(void) {
	po::notify(programOptions);
}

void Configuration::checkArguments(void) {
	if (programOptions.count("help")) {
		showHelp();
	}

	if (!programOptions.count("callsign")) {
		cerr << "Callsign is missing" << endl;
		showHelp();
	}

	if (!programOptions.count("interface_name")) {
		cerr << "TUN interface name is missing" << endl;
		showHelp();
	}
}


