#include "AutorunInfFile.h"
#include <regex>
#include "Parser.h"


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

autorun_inf_file::autorun_inf_file(const char disk_letter)
{
	letter_ = disk_letter;
}


autorun_inf_file::~autorun_inf_file()
{

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
	Parser pars(&inf_file_,100);
	const auto result = pars.parse_loop(key);
	return result[2];
}



