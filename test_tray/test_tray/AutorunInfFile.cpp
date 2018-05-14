#include "AutorunInfFile.h"
#include <regex>


std::cmatch autorun_inf_file::parse_loop(std::string key)
{
	std::cmatch result;
	std::string reg_str = "(";
	reg_str += key;
	reg_str += R"([\s]?={1}[\s]?)([\S\s]*))";
	std::string reg_coment_str = ";";
	const std::regex regular(reg_str.c_str());
	const std::regex rregex(reg_coment_str.c_str());
	while (!inf_file_.eof()) {

		inf_file_.getline(buf_, buf_lenght_);
		std::regex_search(buf_, result, rregex);
		if (!result.empty()) { continue; }
		std::regex_search(buf_, result, regular);

		if (!result.empty()) { break; }
		if (inf_file_.eof()) { return result; }
	}
	return result;
}

std::string autorun_inf_file::build_task(const std::string task) const
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

autorun_inf_file::autorun_inf_file(const char disk_letter, const unsigned delay, const int buf_size)
{
	buf_lenght_ = buf_size;
	buf_ = new char[buf_lenght_];
	letter_ = disk_letter;
}


autorun_inf_file::~autorun_inf_file()
{
	delete[] buf_;
}

int autorun_inf_file::open_file()
{
	std::string path;
	path += letter_;
	path += ":\\autorun.inf";
	inf_file_.open(path.c_str());
	return (!inf_file_.is_open()) ? -1 : 0;
}

bool autorun_inf_file::cheak_file()
{
	switch (this->open_file())
	{
	case -1: return false;
	default: return true;;
	}
}

std::string autorun_inf_file::parse_file(const std::string& key)
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



