#include "Executor.h"


Executor::Executor() : 
invalid_argument("invalid argument"),
file_problem("no autorun file or autorun open property"),
exe_error("error of exequte autorun open file")
{

}

int Executor::exequte_task(std::string task)
{
	if (task.empty()) { return 1; }

	if (system(task.c_str()) != 0)
	{
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
			return -1;
		}
	}
	return 0;

}


const char* Executor::exequte_answer(int i)
{
	switch (i)
	{
	case 0: return ""; break;
	case 1: return file_problem; break;
	case -1: return exe_error; break;
	default: return invalid_argument;
	}
}

Executor::~Executor()
{
	delete invalid_argument;
	delete file_problem;
	delete exe_error;
}
