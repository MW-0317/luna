#include "ArgParser.h"

ArgParser::ArgParser(int argc, char* argv[])
{
	std::vector<std::string> sArgv = std::vector<std::string>();
	for (int i = 0; i < argc; i++)
	{
		std::string cArg = argv[i];
		sArgv.push_back(cArg);
	}
	init(argc, sArgv);
}

ArgParser::ArgParser(int argc, std::vector<std::string> argv)
{
	init(argc, argv);
}

ArgParser::~ArgParser()
{}

void ArgParser::init(int argc, std::vector<std::string> argv)
{
	for (int i = 0; i < argc; i++)
	{
		// Checks if string starts with -- argument grab
		if (i + 1 < argc && argv[i].rfind("--", 0) == 0)
		{
			std::string command = argv[i].substr(2, argv[i].size() - 2);
			std::string argument = argv[i + 1];
			this->argumentPairs.push_back(std::make_pair(command, argument));
		}
	}
}

std::string ArgParser::getArgument(std::string argument)
{
	for (int i = 0; i < argumentPairs.size(); i++)
	{
		// Checks if string starts with -- argument grab
		if (argument == argumentPairs[i].first)
		{
			return argumentPairs[i].second;
		}
	}
	return "";
}