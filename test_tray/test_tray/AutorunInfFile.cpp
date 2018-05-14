#include "AutorunInfFile.h"
#include <regex>
#include "Parser.h"


std::string AutorunFile::build_task(const std::string task) const
{
	if(task.empty())
	{
		return "";
	}
	std::string res;
	res = this->letter_;
	res += ":\\";
	res += task;
	return res;
}

AutorunFile::AutorunFile(const char disk_letter):Parser()
{
	letter_ = disk_letter;
}

/**
 * \brief 
 */
AutorunFile::~AutorunFile()
= default;

int AutorunFile::open_file()
{
	std::string path;
	path += letter_;
	path += ":\\autorun.inf";
	inf_file_.open(path.c_str());
	return (!inf_file_.is_open()) ? -1 : 0;
}

bool AutorunFile::cheak_file()
{
	switch (this->open_file())
	{
	case -1: return false;
	default: return true;;
	}
}

std::string AutorunFile::parse_file(const std::string& key)
{
	if (!inf_file_.is_open())
	{
		if (this->open_file() == -1)
		{
			return std::string ("");
		}
	}
	const auto result = parse_loop(key);
	return result[2];
}



