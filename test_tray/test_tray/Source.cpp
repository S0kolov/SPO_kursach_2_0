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
#include "AutorunInfFile.h"

using namespace std;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL WndProc_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void WndProc_OnDestroy(HWND hWnd);
void AddStatusIcon(HWND hWnd, DWORD dwMessage);
void HandlePopupMenu(HWND hwnd, POINT point);

HINSTANCE hInst;



#define WM_NOTIFYICONMSG (WM_USER + 2)

set<wchar_t> usb_devices;

set<wchar_t> get_flash_drive()
{
	set<wchar_t> devices;

	unsigned int disks = GetLogicalDrives();

	wchar_t drive_root[] = L"?:";

	for(int i =31; i >=0;i--)
	{
		if(disks & (1<< i))
		{
			drive_root[0] = static_cast<wchar_t>('A') + i;
			DWORD type = GetDriveTypeW(drive_root);

			if(type ==DRIVE_REMOVABLE)
			{
				wchar_t buf[260];
				if(QueryDosDeviceW(drive_root,buf,260))
				{
					if(wstring(buf).find(L"\\Floppy") == wstring::npos)
					{
						devices.insert(static_cast<wchar_t>('A') + i);
					}
				}
			}
		}
	}

	return devices;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;
	WNDCLASSEX wndclass;

	usb_devices = get_flash_drive();

	hInst = hInstance;

	wndclass.style = 0;
	wndclass.lpfnWndProc = (WNDPROC)WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "WSCLAS";

	if (!RegisterClassEx(&wndclass))
		if (!RegisterClass((LPWNDCLASS)&wndclass.style))  return FALSE;

	hWnd = CreateWindow("WSCLAS", "", 0, 0, 0, 1, 1, HWND_DESKTOP, NULL, hInst, NULL);

	AddStatusIcon(hWnd, NIM_ADD);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void detected_usb_change()
{
	set<wchar_t> new_device_list = get_flash_drive();
	vector<wchar_t> insert,ejected;

	set_difference(new_device_list.begin(), new_device_list.end(), usb_devices.begin(), usb_devices.end(), back_inserter(insert));
	set_difference(usb_devices.begin(), usb_devices.end(), new_device_list.begin(), new_device_list.end(), back_inserter(ejected));
	for(unsigned int i =0 ; i < insert.size();i++)
	{
		char ch = static_cast<char>(insert[i]);
		//MessageBox(NULL,&ch, "New device connected", MB_OK);
		autorun_inf_file disk(ch);
		disk.exequte_task();
	}
	for (unsigned int i = 0; i< ejected.size(); i++)
	{
		char ch = static_cast<char>(ejected[i]);
		MessageBox(NULL, &ch, "device disconnected", MB_OK);
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

			case WM_RBUTTONDOWN:	// нажатие на иконку правой кнопкой мыши
				GetCursorPos(&pt);	//вычисляем текущее положение курсора
				HandlePopupMenu(hWnd, pt);  //рисуем меню от координат курсора
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
					if(pHdr->dbch_devicetype == DBT_DEVTYP_VOLUME)
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
	PostQuitMessage(0);
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
	lstrcpyn(tnd.szTip, "Example", sizeof(tnd.szTip));
	if( !Shell_NotifyIcon(dwMessage, &tnd))
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
	auto i = TrackPopupMenu(hMenu, TPM_RETURNCMD, point.x, point.y,0, hWnd, nullptr);
	switch(i)
	{
	case 1001: DestroyMenu(hMenu);  WndProc_OnDestroy(hWnd); break;
	case 1002: break;
	default: break;
	}
	DestroyMenu(hMenu);
}