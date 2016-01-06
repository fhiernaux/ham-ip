/*
 * Configuration.h
 *
 *  Created on: Jan 5, 2016
 *      Author: seb
 */

#ifndef SRC_CONFIGURATION_H_
#define SRC_CONFIGURATION_H_

#include <boost/program_options.hpp>
namespace po = boost::program_options;

class Configuration {
public:
	Configuration(int argc, char **argv, std::string fileName);
	virtual ~Configuration();
	const char *getCallsign(void);
	const char *getInterfaceName(void);

protected:
	po::options_description optionDescription;
	po::variables_map programOptions;

	void initializeOptions(void);
	void parseCommandLine(int argc, char **argv);
	void parseFile(std::string fileName);
	void updateStore(void);
	void showHelp(void);
	void checkArguments(void);
};

#endif /* SRC_CONFIGURATION_H_ */
