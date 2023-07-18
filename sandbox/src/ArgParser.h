#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

class ArgParser
{
private:
	// I understand this is not the most efficient solution and instead
	// could use a hashmap. This will be improved at a later time.
	std::vector<std::pair<std::string, std::string>> argumentPairs =
		std::vector<std::pair<std::string, std::string>>();
public:
	void init(int argc, std::vector<std::string> argv);
	ArgParser(int argc, char* argv[]);
	ArgParser(int argc, std::vector<std::string> argv);
	~ArgParser();

	std::string getArgument(std::string argument);
};

