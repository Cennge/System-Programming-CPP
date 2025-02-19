#include <windows.h>
#include <tchar.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{

    // создаём главное окно приложения на основе модального диалога
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR login[20], password[20];
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {             
            GetDlgItemText(hWnd, IDC_EDIT1, login, 20);
            GetDlgItemText(hWnd, IDC_EDIT2, password, 20);

            if (_tcscmp(login, _T("admin")) == 0 && _tcscmp(password, _T("admin")) == 0) {
                MessageBox(hWnd, _T("Админ"), _T("Сообщение"), MB_OK | MB_ICONINFORMATION);
            }
            else {
                MessageBox(hWnd, _T("User"), _T("Сообщение"), MB_OK | MB_ICONINFORMATION);
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