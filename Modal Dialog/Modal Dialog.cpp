#include <windows.h>
#include <tchar.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

void CreateButton(HWND hWnd, int x, int y, int width, int height, int id, LPCWSTR text);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;
    GetWindowRect(hWnd, &rect);
    switch (message)
    {
    case WM_INITDIALOG: 
        CreateButton(hWnd, 50, 50, 100, 100, 1, L"Click Me");
        return TRUE;

    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}

void CreateButton(HWND hWnd, int x, int y, int width, int height, int id, LPCWSTR text)
{
    CreateWindow(
        L"BUTTON", text, WS_CHILD | WS_VISIBLE | BS_CENTER | WS_BORDER,
        x, y, width, height, hWnd,
        (HMENU)id, GetModuleHandle(NULL), NULL);
}
