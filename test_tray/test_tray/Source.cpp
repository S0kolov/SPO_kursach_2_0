#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <windowsx.h>
#include <cstdio>
#include "resource.h"
#include <functional>
#include <Dbt.h>
#include <set>
#include <vector>
#include <algorithm>
#include <conio.h>

#include "white_list.h"
#include "AutorunInfFile.h"
#include "Executor.h"
#include "DeviceManeger.h"
using namespace std;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL WndProc_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void WndProc_OnDestroy(HWND hWnd);
void AddStatusIcon(HWND hWnd, DWORD dwMessage);
void HandlePopupMenu(HWND hwnd, POINT point);

HINSTANCE hInst;

#define WM_NOTIFYICONMSG (WM_USER + 2)

white_list whiteList;
DeviceManeger devMeneger;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASSEX wndclass;

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
	set<device * > new_device_list = devMeneger.get_flash_drive();
	vector<device *> insert, ejected;
	set<device*> usb = devMeneger.get_usb_devices();
	set_difference(new_device_list.begin(), new_device_list.end(),usb.begin(), usb.end(), back_inserter(insert));
	set_difference(usb.begin(), usb.end(), new_device_list.begin(), new_device_list.end(), back_inserter(ejected));
	devMeneger.set_usb_devices(new_device_list);
	for (device * dev : insert)
	{
		AutorunFile disk(dev->letter[0]);
		Executor exe;
		devMeneger.get_flash_drive_info(dev);
		MessageBox(NULL, dev->letter, "New device connected", MB_OK);
		if(whiteList.is_device_trusted(dev))
		{
			std::string message =exe.exequte_answer(exe.exequte_task(disk.build_task(disk.parse_file())));
			if(!message.empty())
			{
				MessageBox(NULL, message.c_str(), dev->letter, MB_OK);
			}
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
				case 6: exe.exequte_task(path);  break;
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