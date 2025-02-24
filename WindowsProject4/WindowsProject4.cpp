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
    static int numbers[20]; // Массив для хранения сгенерированных чисел
    static int count = 0;   // Количество чисел

    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_BUTTON1) {
            HWND hList = GetDlgItem(hWnd, IDC_LIST1);
            SendMessage(hList, LB_RESETCONTENT, 0, 0);

            count = 10 + rand() % 11;
            for (int i = 0; i < count; i++) {
                numbers[i] = (rand() % 10 + 1) * (rand() % 2 ? 1 : -1);

                TCHAR buffer[10];
                _stprintf_s(buffer, _T("%d"), numbers[i]);
                SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buffer);
            }
        }

        if (LOWORD(wParam) == IDC_BUTTON1) {
            int result = 0;
            HWND hStatic = GetDlgItem(hWnd, IDC_STATIC2);

            if (IsDlgButtonChecked(hWnd, IDC_RADIO1) == BST_CHECKED) {
                result = 0;
                for (int i = 0; i < count; i++) result += numbers[i];
            }
            else if (IsDlgButtonChecked(hWnd, IDC_RADIO2) == BST_CHECKED) {
                result = 1;
                for (int i = 0; i < count; i++) result *= numbers[i];
            }
            else if (IsDlgButtonChecked(hWnd, IDC_RADIO3) == BST_CHECKED) {
                if (count > 0) {
                    double avg = 0;
                    for (int i = 0; i < count; i++) avg += numbers[i];
                    avg /= count;

                    TCHAR buffer[20];
                    _stprintf_s(buffer, _T("%.2f"), avg);
                    SetWindowText(hStatic, buffer);
                    return TRUE;
                }
            }

            TCHAR buffer[20];
            _stprintf_s(buffer, _T("%d"), result);
            SetWindowText(hStatic, buffer);
        }
        return TRUE;

    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}
