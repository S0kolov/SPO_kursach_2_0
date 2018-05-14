#pragma once
#include <string>
#include <windows.h>
#include <process.h>

class Executor
{

public:
	Executor();
	int exequte_task(std::string task);
	static const char * exequte_answer(int i);
	~Executor();
};

