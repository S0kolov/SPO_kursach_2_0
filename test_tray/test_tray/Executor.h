#pragma once
#include <string>
#include <Windows.h>
#include <process.h>
class Executor
{
	const char * invalid_argument;
	const char * file_problem;
	const char * exe_error;
public:
	Executor();
	int exequte_task(std::string task);
	const char * exequte_answer(int i);
	~Executor();
};

