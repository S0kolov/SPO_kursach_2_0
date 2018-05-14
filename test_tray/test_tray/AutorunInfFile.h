#pragma once
#include <fstream>
#include <process.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <regex>




class AutorunFile
{
private:
	std::fstream  inf_file_;
	char letter_;

public:
	std::string build_task(std::string task) const;
	explicit AutorunFile(char disk_letter);
	~AutorunFile();
	int  open_file();
	bool cheak_file();
	std::string parse_file(const std::string& key = "open");
};
