#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <string>

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

HWND hStart;

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_INITDIALOG:
        hStart = GetDlgItem(hWnd, IDC_BUTTON1);
        return TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            CreateMutex(NULL, FALSE, TEXT("{9509D0D4-3552-4E02-B278-6A6E8F97206B}"));

            STARTUPINFO si = { sizeof(si) };
            PROCESS_INFORMATION pi;
            TCHAR filename[MAX_PATH];

            _tcscpy_s(filename, TEXT("Write.exe"));

            if (!CreateProcess(NULL, filename, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                return FALSE;
            }

            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);

            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);

            _tcscpy_s(filename, TEXT("Read.exe"));

            if (!CreateProcess(NULL, filename, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                return FALSE;
            }
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
        return TRUE;

    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}