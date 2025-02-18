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
    int score;
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            wchar_t buffer[20];
            swprintf(buffer, 20, L"%d/100", score);
            SetWindowText(GetDlgItem(hWnd, IDC_STATIC1), buffer);
        }
        if (LOWORD(wParam) == IDC_RADIO6) SetWindowText(GetDlgItem(hWnd, IDC_STATIC1), L"2/100");
    case WM_SHOWWINDOW:
        //MessageBox(0, TEXT("Test"), 0, MB_OK);
        return TRUE;
    case WM_CLOSE:
        EndDialog(hWnd, 0); // закрываем модальный диалог
        return TRUE;
    }
    return FALSE;
}