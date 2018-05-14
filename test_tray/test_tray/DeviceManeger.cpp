#include "DeviceManeger.h"



DeviceManeger::DeviceManeger()
{
	usb_devices = get_flash_drive();
	file = fopen("conectinglogFile.txt", "a+");
}

std::set<device*> DeviceManeger::get_usb_devices()
{
	return usb_devices;
}

void DeviceManeger::set_usb_devices(std::set<device*> dev)
{
	usb_devices = dev;
}

WCHAR* DeviceManeger::get_elem(WCHAR* ptr, WCHAR delim, WCHAR* buf)
{
	WCHAR *ptr1 = NULL;
	ptr1 = wcschr(ptr, delim);
	if (ptr1)
	{
		wcsncpy(buf, ptr, (int)(ptr1 - ptr));
		buf[(int)(ptr1 - ptr)] = 0;
		return (ptr1 + 1);
	}
	else
	{
		wcscpy(buf, ptr);
		return NULL;
	}
}

std::set<device*> DeviceManeger::get_flash_drive()
{
	std::set<device *> devices;

	unsigned int disks = GetLogicalDrives();

	wchar_t drive_root[] = L"?:";

	for (int i = 31; i >= 0; i--)
	{
		if (disks & (1 << i))
		{
			drive_root[0] = static_cast<wchar_t>('A') + i;
			DWORD type = GetDriveTypeW(drive_root);

			if (type == DRIVE_REMOVABLE)
			{
				wchar_t buf[260];
				if (QueryDosDeviceW(drive_root, buf, 260))
				{
					if (std::wstring(buf).find(L"\\Floppy") == std::wstring::npos)
					{
						device * dev = new device;
						dev->letter[0] = 'A' + i;
						dev->letter[1] = '\0';
						devices.insert(dev);
					}
				}
			}
		}
	}

	return devices;
}

void DeviceManeger::get_flash_drive_info(device* dev)

{
	HKEY hk = NULL;
	LONG Ret = 0;
	HANDLE hdrive = INVALID_HANDLE_VALUE;
	DWORD data_size, dtype;
	WCHAR *data = NULL, *buf1 = NULL, *buf2 = NULL;
	WCHAR *ptr1_1 = NULL, *ptr1_2 = NULL, *ptr2_1 = NULL, *ptr2_2 = NULL;
	CHAR dos_link[50];
	CHAR drive_name2[20];
	__try
	{
		wsprintf(drive_name2, "\\\\.\\%c:", dev->letter[0]);
		hdrive = CreateFile(drive_name2, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hdrive == INVALID_HANDLE_VALUE)
		{
			fprintf(file, "error get_flash_drive_info: error CreateFile: %u\n\n", GetLastError());
			__leave;
		}
		CloseHandle(hdrive);

		Ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\MountedDevices", 0, KEY_READ, &hk);
		if (Ret != ERROR_SUCCESS)
		{
			fprintf(file, "error get_flash_drive_info: error RegOpenKeyEx: %u\n\n", Ret);
			__leave;
		}

		wsprintf(dos_link, "\\DosDevices\\%c:", dev->letter[0]);
		Ret = RegQueryValueEx(hk, dos_link, NULL, &dtype, NULL, &data_size);
		if (Ret == ERROR_SUCCESS)
		{
			data = (WCHAR *)malloc(data_size + 2);
			buf1 = (WCHAR *)malloc(data_size + 2);
			buf2 = (WCHAR *)malloc(data_size + 2);
			if (data == NULL || buf1 == NULL || buf2 == NULL)
			{
				fprintf(file, "Error get_flash_drive_info: error malloc.\n\n");
				__leave;
			}
			memset(data, 0, data_size + 2);

			Ret = RegQueryValueEx(hk, dos_link, NULL, &dtype, (LPBYTE)data, &data_size);
			if (Ret != ERROR_SUCCESS)
			{
				fprintf(file, "error get_flash_drive_info: error RegQueryValueEx: %u\n\n", Ret);
				__leave;
			}
			if (wcsstr(data, L"USBSTOR") != NULL)
			{
				fprintf(file, "Date about flsh-drive %c:\n", dev->letter[0]);
				ptr1_1 = get_elem(data, L'#', buf1);
				if (ptr1_1)
				{
					ptr1_2 = get_elem(ptr1_1, L'#', buf1);
					ptr1_1 = buf1;
					do
					{
						ptr2_2 = get_elem(ptr1_1, L'&', buf2);
						if (wcsstr(buf2, L"Ven_"))
						{
							fprintf(file, "Manufacture : %ls\n", buf2 + 4);
							dev->manufacture = buf2 + 4;
						}
						else if (wcsstr(buf2, L"Prod_"))
						{
							fprintf(file, "Name of product : %ls\n", buf2 + 5);
							dev->name_of_product = buf2 + 5;
						}
						ptr1_1 = ptr2_2;
					} while (ptr2_2);

					ptr1_1 = get_elem(ptr1_2, L'#', buf1);
					ptr1_1 = buf1;
					do
					{
						ptr2_2 = get_elem(ptr1_1, L'&', buf2);
						if (wcslen(buf2)>3)
						{
							fprintf(file, "serial number : %ls\n", buf2);
							dev->serial_number = buf2;
							break;
						}
						ptr1_1 = ptr2_2;
					} while (ptr2_2);
				}
			}//if USBSTOR
			else
				fprintf(file, "Disk %c: not a flash-drive\n", dev->letter[0]);
		}
		else
			fprintf(file, "ERROR get_flash_drive_info: error RegQueryValueEx: %u\n", Ret);

		fprintf(file, "\n");
	}
	__finally
	{
		//if (data != nullptr) free(data);
		//if (buf1 != nullptr) free(buf1);
		//if (buf2 != nullptr) free(buf2);
		//if (hk != nullptr) RegCloseKey(hk);
	}
}

DeviceManeger::~DeviceManeger()
{
	fclose(file);
}
