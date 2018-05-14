#pragma once
#include <string>
#include <regex>
#include <fstream>

class Parser
{
	std::string key;
	int buf_lenght_;
	char * buf_;
	std::fstream  *inf_file_;
public:
	Parser(std::fstream * file, int buf_size = 100);
	std::cmatch parse_loop(std::string key);

	~Parser();
};

