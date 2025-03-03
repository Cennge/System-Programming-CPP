#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <commctrl.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
bool isBinary;

using namespace std;

void StartCopying(HWND hWnd);
DWORD WINAPI CopyFileThread(LPVOID lpParam);

struct CopyData {
    HWND hWnd;
    char sourcePath[MAX_PATH];
    char pastePath[MAX_PATH];
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_CHECK1) {
            isBinary = false;
            if (IsDlgButtonChecked(hWnd, IDC_CHECK1)) {
                CheckDlgButton(hWnd, IDC_CHECK2, BST_UNCHECKED);
            }
        }
        if (LOWORD(wParam) == IDC_CHECK2) {
            isBinary = true;
            if (IsDlgButtonChecked(hWnd, IDC_CHECK2)) {
                CheckDlgButton(hWnd, IDC_CHECK1, BST_UNCHECKED);
            }
        }
        if (wParam == IDC_BUTTON1) {
           if(isBinary = true) StartCopying(hWnd);
            return TRUE;
        }
    case WM_SHOWWINDOW:
        return TRUE;
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}

void StartCopying(HWND hWnd)
{
    CopyData* data = new CopyData;
    data->hWnd = hWnd;

    GetDlgItemTextA(hWnd, IDC_EDIT1, data->sourcePath, MAX_PATH);
    GetDlgItemTextA(hWnd, IDC_EDIT2, data->pastePath, MAX_PATH);

    CreateThread(NULL, 0, CopyFileThread, data, 0, NULL);
}

DWORD WINAPI CopyFileThread(LPVOID lpParam)
{
    CopyData* data = (CopyData*)lpParam;

    wchar_t src[256];
    GetDlgItemText(data->hWnd, IDC_EDIT1, src, 256);
    ofstream paste(data->pastePath, ios::binary);

    if (src && paste) {
        int i = 0;
        while (i<256) {
            paste.put(src[i]);
            i++;
        }
        MessageBoxA(data->hWnd, "Файл скопирован", "Ура", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBoxA(data->hWnd, "Ошибка при открытии файлов :(((", "Ошибка", MB_OK | MB_ICONERROR);
    }

    delete data;
    return 0;
}
