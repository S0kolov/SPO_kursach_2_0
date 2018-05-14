#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <windowsx.h>
#include <cstdio>
#include <iostream>
#include "resource.h"
#include <functional>
#include <Dbt.h>
#include <set>
#include <vector>
#include <algorithm>
#include <conio.h>

#include "white_list.h"
#include "AutorunInfFile.h"
using namespace std;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL WndProc_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void WndProc_OnDestroy(HWND hWnd);
void AddStatusIcon(HWND hWnd, DWORD dwMessage);
void HandlePopupMenu(HWND hwnd, POINT point);

HINSTANCE hInst;



#define WM_NOTIFYICONMSG (WM_USER + 2)

FILE * file;
white_list whiteList;
set<device *> usb_devices;

WCHAR* GetElem(WCHAR *ptr, WCHAR delim, WCHAR *buf)
{
	WCHAR *ptr1 = NULL;
	ptr1 = wcschr(ptr, delim);
	if (ptr1)
	{
		wcsncpy(buf,  ptr, (int)(ptr1 - ptr));
		buf[(int)(ptr1 - ptr)] = 0;
		return (ptr1 + 1);
	}
	else
	{
		wcscpy(buf, ptr);
		return NULL;
	}
}

void get_flash_drive_info(device * dev)
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
		//проверяем существует ли диск в системе
		wsprintf(drive_name2, "\\\\.\\%c:", dev->letter[0]);
		hdrive = CreateFile(drive_name2, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (hdrive == INVALID_HANDLE_VALUE)
		{
			fprintf(file,"error get_flash_drive_info: error CreateFile: %u\n\n", GetLastError());
			__leave;
		}
		CloseHandle(hdrive);

		Ret = RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\MountedDevices", 0, KEY_READ, &hk);
		if (Ret != ERROR_SUCCESS)
		{
			fprintf(file,"error get_flash_drive_info: error RegOpenKeyEx: %u\n\n", Ret);
			__leave;
		}

		wsprintf(dos_link,"\\DosDevices\\%c:", dev->letter[0]);
		Ret = RegQueryValueEx(hk, dos_link, NULL, &dtype, NULL, &data_size);
		if (Ret == ERROR_SUCCESS)
		{
			data = (WCHAR *)malloc(data_size + 2);
			buf1 = (WCHAR *)malloc(data_size + 2);
			buf2 = (WCHAR *)malloc(data_size + 2);
			if (data == NULL || buf1 == NULL || buf2 == NULL)
			{
				fprintf(file,"Error get_flash_drive_info: error malloc.\n\n");
				__leave;
			}
			memset(data, 0, data_size + 2);

			Ret = RegQueryValueEx(hk, dos_link, NULL, &dtype, (LPBYTE)data, &data_size);
			if (Ret != ERROR_SUCCESS)
			{
				fprintf(file,"error get_flash_drive_info: error RegQueryValueEx: %u\n\n", Ret);
				__leave;
			}
			if (wcsstr(data, L"USBSTOR") != NULL)
			{
				fprintf(file,"Date about flsh-drive %c:\n", dev->letter[0]);
				ptr1_1 = GetElem(data, L'#', buf1);
				if (ptr1_1)
				{
					ptr1_2 = GetElem(ptr1_1, L'#', buf1);
					ptr1_1 = buf1;
					do
					{
						ptr2_2 = GetElem(ptr1_1, L'&', buf2);
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

					ptr1_1 = GetElem(ptr1_2, L'#', buf1);
					ptr1_1 = buf1;
					do
					{
						ptr2_2 = GetElem(ptr1_1, L'&', buf2);
						if (wcslen(buf2)>3)
						{
							fprintf(file,"serial number : %ls\n", buf2);
							dev->serial_number = buf2;
							break;
						}
						ptr1_1 = ptr2_2;
					} while (ptr2_2);
				}
			}//if USBSTOR
			else
				fprintf(file ,"Disk %c: not a flash-drive\n", dev->letter[0]);
		}
		else
			fprintf(file,"ERROR get_flash_drive_info: error RegQueryValueEx: %u\n", Ret);

		fprintf(file,"\n");
	}
	__finally
	{
		//if (data != nullptr) free(data);
		//if (buf1 != nullptr) free(buf1);
		//if (buf2 != nullptr) free(buf2);
		//if (hk != nullptr) RegCloseKey(hk);
	}
}

set<device* > get_flash_drive()
{
	set<device *> devices;

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
					if (wstring(buf).find(L"\\Floppy") == wstring::npos)
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	file = fopen("conectinglogFile.txt", "a+");
	if (file == NULL) { return -1; }
	MSG msg;
	WNDCLASSEX wndclass;

	usb_devices = get_flash_drive();

	hInst = hInstance;

	wndclass.style = 0;
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = nullptr;
	wndclass.hCursor = nullptr;
	wndclass.hbrBackground = nullptr;
	wndclass.lpszMenuName = nullptr;
	wndclass.lpszClassName = "WSCLAS";

	if (!RegisterClassEx(&wndclass))
		if (!RegisterClass((LPWNDCLASS)&wndclass.style))  return FALSE;

	HWND hWnd = CreateWindow("WSCLAS", "", 0, 0, 0, 1, 1, HWND_DESKTOP, NULL, hInst, NULL); //CreateWindow("WSCLAS", "AUtorun\0", WS_VISIBLE, 0, 0, 10, 10, HWND_DESKTOP, NULL, hInst, NULL);

	AddStatusIcon(hWnd, NIM_ADD);

	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void detected_usb_change()
{
	set<device * > new_device_list = get_flash_drive();
	vector<device *> insert, ejected;
	set_difference(new_device_list.begin(), new_device_list.end(), usb_devices.begin(), usb_devices.end(), back_inserter(insert));
	set_difference(usb_devices.begin(), usb_devices.end(), new_device_list.begin(), new_device_list.end(), back_inserter(ejected));
	usb_devices = new_device_list;
	for (device * dev : insert)
	{
		autorun_inf_file disk(dev->letter[0]);
		get_flash_drive_info(dev);
		MessageBox(NULL, dev->letter, "New device connected", MB_OK);
		if(whiteList.is_device_trusted(dev))
		{
			disk.exequte_task();
			break;
		}
		else {
			string path = disk.parse_file();
			char device_name[] = "device _ \0";
			device_name[7] = dev->letter[0];
			if (path.empty())
			{
				MessageBox(NULL, "no autorun file or autorun open property", device_name, MB_OK);
			}
			else
			{
				int i = MessageBox(NULL, "disk have a autorun file. Do you want to exequ it?", device_name, MB_YESNO);
				switch (MessageBox(NULL, "disk have a autorun file. Add it to white list?", device_name, MB_YESNO))
				{
				case 6: whiteList.add_device_to_white_list(dev);  break;
				case 7: break;
				}
				switch (i)
				{
				case 6: disk.exequte_task(path);  break;
				case 7: break;
				}
			}
		}
	}
	for (device * dev : ejected)
	{
		MessageBox(NULL, dev->letter, "device disconnected", MB_OK);
	}
}


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PDEV_BROADCAST_HDR pHdr;
	POINT pt;
	switch (msg) {
	case WM_NOTIFYICONMSG:
		switch (lParam) {
		case WM_LBUTTONDBLCLK:
			MessageBox(NULL, "double click", "message from icon", MB_OK);
			break;

		case WM_RBUTTONDOWN:	
			GetCursorPos(&pt);	
			HandlePopupMenu(hWnd, pt);  
			break;

		default:
			break;
		}
		break;

		HANDLE_MSG(hWnd, WM_CREATE, WndProc_OnCreate);		//стандартный обработчик создания окна
		HANDLE_MSG(hWnd, WM_DESTROY, WndProc_OnDestroy);	//стандартный обработчик уничтожения окна

	case WM_DEVICECHANGE:
		switch (wParam) {
		case DBT_DEVICEARRIVAL:
			pHdr = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
			if (pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
			{
				detected_usb_change();
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			pHdr = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
			if (pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
			{
				detected_usb_change();
			}
			break;
		}
		break;
	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return 0;
}


BOOL WndProc_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	return TRUE;
}

void WndProc_OnDestroy(HWND hWnd)
{
	int ianswer = MessageBox(NULL, "Are you sure that you want to close the application?", "close app", MB_YESNO);
	switch (ianswer)
	{
	case 6: PostQuitMessage(0);  break;
	case 7: return;
	}
}

void AddStatusIcon(HWND hWnd, DWORD dwMessage)
{
	HICON hStatusIcon;			// Хэндл иконки в статус-баре
	LPCSTR pszIDStatusIcon;		// Указатель на Иконку в статус-баре
	NOTIFYICONDATA tnd;

	pszIDStatusIcon = MAKEINTRESOURCE(IDI_ICON);

	hStatusIcon = LoadIcon(hInst, pszIDStatusIcon);
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = hWnd;
	tnd.uID = 1;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = WM_NOTIFYICONMSG;
	tnd.hIcon = hStatusIcon;
	lstrcpyn(tnd.szTip, "Autorun", sizeof(tnd.szTip));
	if (!Shell_NotifyIcon(dwMessage, &tnd))
	{
		printf("icon error");
		return;
	}
}


void HandlePopupMenu(HWND hWnd, POINT point)
{

	HMENU hMenu = CreatePopupMenu();

	AppendMenu(hMenu, MF_STRING, 1001, "close");
	AppendMenu(hMenu, MF_STRING, 1002, "hello Kate =)");

	if (!hMenu)  return;
	auto i = TrackPopupMenu(hMenu, TPM_RETURNCMD, point.x, point.y, 0, hWnd, nullptr);
	switch (i)
	{
	case 1001: WndProc_OnDestroy(hWnd); break;
	case 1002: break;
	default: break;
	}
	DestroyMenu(hMenu);
}