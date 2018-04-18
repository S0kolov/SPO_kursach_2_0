#include "AutorunInfFile.h"
#include <regex>


std::cmatch autorun_inf_file::parse_loop(std::string key)
{
	std::cmatch result;
	std::string reg_str = "(";
	reg_str += key;
	reg_str += R"([\s]?={1}[\s]?)([\S\s]*))";
	const std::regex regular(reg_str.c_str());
	while (!inf_file_.eof()) {
		inf_file_.getline(buf_, buf_lenght_);
		std::regex_search(buf_, result, regular);
		if (!result.empty()) { break; }
		if (inf_file_.eof()) { return result; }
	}
	return result;
}

std::string autorun_inf_file::build_task(const std::string task) const
{
	std::string res;
	res = this->letter_;
	res += ":\\";
	res += task;
	return res;
}

autorun_inf_file::autorun_inf_file(const char disk_letter, const unsigned delay, const int buf_size)
{
	//bufSie validation
	buf_lenght_ = buf_size;
	buf_ = new char[buf_lenght_];
	letter_ = disk_letter;
	statistic_.delay = delay;
	statistic_.rememnber = false;

	if(statistic_.rememnber) { exequte_task(parse_file()); }
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
	if(!inf_file_.is_open())
	{
		if (this->open_file() == -1) { return nullptr; }
	}
	const auto result = parse_loop(key);
	return result[2];
}

int autorun_inf_file::exequte_task(std::string task)
{
	if (task.empty()) { task = this->parse_file(); }

	if(system(task.c_str())!=0)
	{
		task = build_task(task);
		STARTUPINFO startup_info;
		PROCESS_INFORMATION proc_info;
		ZeroMemory(&startup_info, sizeof(startup_info));
		startup_info.cb = sizeof(startup_info);
		ZeroMemory(&proc_info, sizeof(proc_info));

		if (!CreateProcess(task.c_str(),
			nullptr,
			nullptr,
			nullptr,
			FALSE,
			CREATE_NEW_CONSOLE,
			nullptr,
			nullptr,
			&startup_info,
			&proc_info))
		{
			//std::cout << "ERROR with proc" << std::endl;
			return -1;
		}
	}
	return 0;
	
}
