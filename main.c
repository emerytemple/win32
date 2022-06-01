
// cmake -S . -B build
// cl /Tc win32_main.c Gdi32.lib User32.lib

#include <Windows.h>
#include <strsafe.h>
#include <string.h>

char debug_string[256];

LRESULT CALLBACK main_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void process_input(UINT message, WPARAM wparam, LPARAM lparam);
void ErrorExit(LPTSTR lpszFunction);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int window_control)
{
	char str[256];
	sprintf_s(str, sizeof(str), "It works! - number: %d\n", 10);
	OutputDebugString(str);

	WNDCLASSEX window_info = {
		.cbSize = sizeof(WNDCLASSEX),
		.style = CS_HREDRAW | CS_VREDRAW,
		.lpfnWndProc = (WNDPROC)main_callback,
		.cbClsExtra = 0,
		.cbWndExtra = 0,
		.hInstance = instance,
		.hIcon = LoadIcon(NULL, IDI_APPLICATION),
		.hCursor = LoadCursor(NULL, IDC_ARROW),
		.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH),
		.lpszMenuName = 0,
		.lpszClassName = "ClassName",
		.hIconSm = NULL,
	};

	RegisterClassEx(&window_info);

	HWND window = CreateWindow(
		window_info.lpszClassName,
		"WindowName",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		instance,
		NULL
	);

	if (!window) ErrorExit(TEXT("CreateWindow"));

	ShowWindow(window, window_control);
	UpdateWindow(window);

	MSG msg = {
		.message = WM_NULL,
	};

	BOOL running = 1;

	while (running)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				running = 0;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// get input
		
	}

	return msg.wParam;
}

LRESULT CALLBACK main_callback(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;

	switch (message) {
		case WM_KEYDOWN:
			process_input(message, wparam, lparam);
			InvalidateRect(window, NULL, 1);
			UpdateWindow(window);
		case WM_KEYUP:
			break;
		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(window, &ps);
			RECT rec;
			GetClientRect(window, &rec);
			const char *str = "out";
			DrawText(hdc, TEXT(debug_string), strlen(debug_string), &rec, DT_TOP | DT_LEFT);
			EndPaint(window, &ps);
			ReleaseDC(window, hdc);
			break;
		case WM_CLOSE:
			DestroyWindow(window);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProc(window, message, wparam, lparam);
	}

	return result;
}

void process_input(UINT message, WPARAM wparam, LPARAM lparam)
{
	WORD vk_code = LOWORD(wparam);

	WORD key_flags = HIWORD(lparam);
	WORD repeat_count = LOWORD(lparam);

	WORD scan_code = LOBYTE(key_flags);

	BOOL extended_key = (key_flags & KF_EXTENDED) == KF_EXTENDED;
	BOOL dialog_mode = (key_flags & KF_DLGMODE) == KF_DLGMODE;
	BOOL menu_mode = (key_flags & KF_MENUMODE) == KF_MENUMODE;
	BOOL context_code = (key_flags & KF_ALTDOWN) == KF_ALTDOWN;
	BOOL previous_key_state = (key_flags & KF_REPEAT) == KF_REPEAT;
	BOOL transition_state = (key_flags & KF_UP) == KF_UP;


	sprintf_s(debug_string, sizeof(debug_string), "scan = %d, vk = %d, previous = %d, transition = %d, count = %d\n", scan_code, vk_code, previous_key_state, transition_state, repeat_count);

/*
	switch(wparam)
	{
		case VK_UP:
			strcpy(debug_string, "up");
			break;
		case VK_DOWN:
			strcpy(debug_string, "down");
			break;
		case VK_LEFT:
			strcpy(debug_string, "left");
			break;
		case VK_RIGHT:
			strcpy(debug_string, "right");
			break;
		default:
			break;
	}
*/
}

void ErrorExit(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);

	// Display the error message and exit the process

	int len = lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40;
	lpDisplayBuf = (LPVOID)LocalAlloc( LMEM_ZEROINIT, len * sizeof(TCHAR) );

	StringCchPrintf(
		(LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf
	);

	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

	ExitProcess(dw); 
}




















