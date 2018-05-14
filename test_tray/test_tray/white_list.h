#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <windows.h>

struct device
{
	char letter[2];
	WCHAR * manufacture;
	WCHAR * name_of_product;
	WCHAR * serial_number;
};

class white_list
{
	FILE * _file;
	const char  _file_path[14];
public:
	white_list();
	const char* get_file_path();
	void add_device_to_white_list(struct device * dev);
	bool is_device_trusted(struct device * dev);
	void delete_device_from_white_list(struct evice * dev);
	~white_list();
};

