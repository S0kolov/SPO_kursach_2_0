#pragma once
#include <fstream>
#include <process.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <regex>

class autorun_inf_file
{
private:
	std::fstream  inf_file_;
	char letter_;

public:
	std::string build_task(std::string task) const;
	explicit autorun_inf_file(char disk_letter);
	~autorun_inf_file();
	int  open_file();
	bool cheak_file();
	std::string parse_file(const std::string& key = "open");
};
