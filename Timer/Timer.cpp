#include <windows.h>
#include <tchar.h>
#include <time.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szClassWindow[] = TEXT(" аркасное приложение");	

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG Msg;
	WNDCLASSEX wcl;
	wcl.cbSize = sizeof(wcl);	
	wcl.style = CS_HREDRAW | CS_VREDRAW;	
	wcl.lpfnWndProc = WindowProc;	
	wcl.cbClsExtra = 0;	
	wcl.cbWndExtra = 0; 	
	wcl.hInstance = hInst;	
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);	
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); 
	wcl.lpszMenuName = NULL;	
	wcl.lpszClassName = szClassWindow;	
	wcl.hIconSm = NULL;	
	if (!RegisterClassEx(&wcl))
		return 0;
	hWnd = CreateWindowEx(0, szClassWindow, TEXT("–абота с таймером"), WS_OVERLAPPEDWINDOW,	
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while(GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);	
		DispatchMessage(&Msg);	
	}
	return Msg.wParam;
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	static time_t t;
	static TCHAR str[100];
	t = time(NULL);	// количество секунд, прошедших с 01.01.1970
	lstrcpy(str, _tctime(&t));	// формирование строки следующего формата:  день мес€ц число часы:минуты:секунды год
	str[lstrlen(str) - 1] = '\0';
	SetWindowText(hwnd, str);	// вывод даты и времени в заголовок окна

	RECT rect;
	GetWindowRect(hwnd, &rect);
	SetWindowPos(hwnd, nullptr, rect.left + 5, rect.top, 0, 0, SWP_NOSIZE);
}

LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY: 
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			if (wParam == VK_RETURN) MoveWindow(hWnd, 0, 0, 300, 300, true);
			
			else if(wParam == VK_ESCAPE) KillTimer(hWnd, 1);

			else if (wParam == VK_LEFT) {
				RECT rect;
				GetWindowRect(hWnd, &rect);
				SetWindowPos(hWnd, nullptr, rect.left - 5, rect.top, 0, 0, SWP_NOSIZE);
			}

			else if (wParam == VK_RIGHT) {
				RECT rect;
				GetWindowRect(hWnd, &rect);
				SetWindowPos(hWnd, nullptr, rect.left + 5, rect.top, 0, 0, SWP_NOSIZE);
			}

			else if (wParam == VK_UP) {
				RECT rect;
				GetWindowRect(hWnd, &rect);
				SetWindowPos(hWnd, nullptr, rect.left, rect.top - 5, 0, 0, SWP_NOSIZE);
			}

			else if (wParam == VK_DOWN) {
				RECT rect;
				GetWindowRect(hWnd, &rect);
				SetWindowPos(hWnd, nullptr, rect.left, rect.top +5, 0, 0, SWP_NOSIZE);
			}

			break;	

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

