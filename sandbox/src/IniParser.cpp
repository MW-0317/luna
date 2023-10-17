#include "IniParser.h"

#include <fstream>
#include <iostream>
#include <vector>

void IniParser::init(std::string filename)
{
	std::string line;
	std::ifstream meshFile(filename);

	if (!meshFile.is_open())
	{
		std::cout << "IniParser\n\t^Could not open file" << std::endl;
	}

	std::map<std::string, std::string> category;
	std::string categoryName = "";
	while (std::getline(meshFile, line))
	{
		if (line[0] == '\0') continue;
		std::vector<std::string> lineData;
		size_t i = 0;
		size_t next = line.find(' ');
		while (next != std::string::npos)
		{
			std::string sub = line.substr(i, next - i);
			if (sub != "" && sub != " ")
				lineData.push_back(sub);
			i = next + 1;
			next = line.find(' ', i);
		}
		std::string sub = line.substr(i, line.size() - i);
		if (sub != "" && sub != " ")
			lineData.push_back(line.substr(i, line.size() - i));

		if (lineData[0][0] == '[' && lineData[0][lineData[0].size() - 1] == ']')
		{
			initVals.emplace(categoryName, category);
			category = std::map<std::string, std::string>();
			categoryName = lineData[0].substr(1, lineData[0].size() - 2);
		}
		else if (lineData.size() > 1)
		{
			category.emplace(lineData[0], lineData[2]);
		}
	}
	initVals.emplace(categoryName, category);

}

IniParser::IniParser(std::string filename)
{
	init(filename);
}

std::map<std::string, std::map<std::string, std::string>> IniParser::getMap()
{
	return initVals;
}