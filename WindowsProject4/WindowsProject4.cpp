#include <windows.h>
#include <tchar.h>
#include <ctime>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    srand((unsigned)time(0));
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            HWND hList = GetDlgItem(hWnd, IDC_LIST1);

            SendMessage(hList, LB_RESETCONTENT, 0, 0);

            int count = 10 + rand() % 11;

            for (int i = 0; i < count; i++) {
                int number;
                do {
                    number = -10 + rand() % 21;
                } while (number == 0);

                TCHAR buffer[10];
                _stprintf_s(buffer, _T("%d"), number);

                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buffer);
            }
        }
        return TRUE;
    case WM_SHOWWINDOW:
        return TRUE;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}