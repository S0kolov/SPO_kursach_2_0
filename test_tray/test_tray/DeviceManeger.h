#pragma once
#include "white_list.h"
#include <set>

class DeviceManeger
{
	FILE * file;
	std::set<device *>  usb_devices;
	static WCHAR* get_elem(WCHAR *ptr, WCHAR delim, WCHAR *buf);
public:
	DeviceManeger();
	std::set<device*> get_usb_devices();
	void set_usb_devices(std::set<device*> dev);
	std::set<device* > get_flash_drive();
	static void get_flash_drive_info(device * dev);

	~DeviceManeger();
};

