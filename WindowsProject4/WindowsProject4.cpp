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
    static int score = 0;
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            score = 0;

            if (IsDlgButtonChecked(hWnd, IDC_RADIO6) == BST_CHECKED) score += 20;
            if (IsDlgButtonChecked(hWnd, IDC_RADIO1) == BST_CHECKED) score += 20;
            if (IsDlgButtonChecked(hWnd, IDC_RADIO8) == BST_CHECKED) score += 20;
            if (IsDlgButtonChecked(hWnd, IDC_CHECK1) == BST_CHECKED) score += 10;
            if (IsDlgButtonChecked(hWnd, IDC_CHECK3) == BST_CHECKED) score += 10;
            if (IsDlgButtonChecked(hWnd, IDC_CHECK4) == BST_CHECKED) score += 10;
            if (IsDlgButtonChecked(hWnd, IDC_CHECK5) == BST_CHECKED) score += 10;

            wchar_t buffer[20];
            swprintf(buffer, 20, L"%d/100", score);
            SetWindowText(GetDlgItem(hWnd, IDC_STATIC1), buffer);
        }
    case WM_SHOWWINDOW:
        return TRUE;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}