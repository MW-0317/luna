#pragma once
#include <map>
#include <string>

class IniParser
{
private:
	std::map<std::string, std::map<std::string, std::string>> initVals;
public:
	void init(std::string filename);
	IniParser(std::string filename);
	~IniParser() {};

	std::map<std::string, std::map<std::string, std::string>> getMap();
};

