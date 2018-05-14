#include "Executor.h"


Executor::Executor()
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
	case 1: return "no autorun file or no autorun open propertis"; break;
	case -1: return "error of exequte autorun open file"; break;
	default: return "invalid argument";
	}
}

Executor::~Executor()
{
}
