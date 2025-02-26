#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <ctime>

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI FilesRandomNum(LPVOID lpParam);

const TCHAR* fileNames[] = { _T("1.txt"), _T("2.txt"), _T("3.txt") };

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            EndDialog(hWnd, 0);
        }
        else if (LOWORD(wParam) == IDC_BUTTON2) {
            srand((unsigned int)time(NULL));
            for (int i = 0; i < 3; i++) {
                CreateThread(NULL, 0, FilesRandomNum, (LPVOID)i, 0, NULL);
            }
        }
        break;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}

DWORD WINAPI FilesRandomNum(LPVOID lpParam)
{
    int fileIndex = (int)lpParam;
    HANDLE hFile = CreateFile(fileNames[fileIndex], GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        TCHAR buffer[20];
        for (int i = 0; i < 10; i++) {
            wsprintf(buffer, _T("%d\r\n"), rand() % 100);
            WriteFile(hFile, buffer, lstrlen(buffer) * sizeof(TCHAR), &bytesWritten, NULL);
        }
        CloseHandle(hFile);
    }
    return 0;
}
