#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <fstream>
#include <ctime>

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI FilesRandomNum(LPVOID lpParam);

using namespace std;

const TCHAR* fileNames[] = { _T("1.txt"), _T("2.txt"), _T("3.txt") };

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM)
{
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            EndDialog(hWnd, 0);
        }
        else if (LOWORD(wParam) == IDC_BUTTON2) {
            srand(static_cast<unsigned int>(time(NULL)));
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
    ofstream file(fileNames[fileIndex], ios::app);//append mode
    if (file.is_open()) {
        for (int i = 0; i < 10; i++) {
            file << rand() % 100 << "\n";
        }
    }
    return 0;
}

