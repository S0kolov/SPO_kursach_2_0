#pragma once

#include <string>
#include <regex>
#include "Parser.h"




class AutorunFile: public Parser
{
	char letter_;
public:
	std::string build_task(std::string task) const;
	explicit AutorunFile(char disk_letter);
	~AutorunFile();
	int  open_file();
	bool cheak_file();
	std::string parse_file(const std::string& key = "open");
};
