#include <windows.h>
#include <fstream>
#include "resource.h"

using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI CopyFileThread(LPVOID lpParam);

struct CopyData {
    HWND hWnd;
    char sourcePath[MAX_PATH];
    char pastePath[MAX_PATH];
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

void StartCopying(HWND hWnd)
{
    CopyData* data = new CopyData;
    data->hWnd = hWnd;

    GetDlgItemTextA(hWnd, IDC_EDIT1, data->sourcePath, MAX_PATH);
    GetDlgItemTextA(hWnd, IDC_EDIT3, data->pastePath, MAX_PATH);

    CreateThread(NULL, 0, CopyFileThread, data, 0, NULL);
}

DWORD WINAPI CopyFileThread(LPVOID lpParam)
{
    CopyData* data = (CopyData*)lpParam;

    ifstream src(data->sourcePath, ios::binary);
    ofstream paste(data->pastePath, ios::binary);

    if (src && paste) {
        char ch;
        while (src.get(ch)) { 
            Sleep(10);
            paste.put(ch);      
        }
        MessageBoxA(data->hWnd, "Файл скопирован", "Ура", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBoxA(data->hWnd, "Ошибка при открытии файлов :((((", "Ошибка", MB_OK | MB_ICONERROR);
    }

    delete data;
    return 0;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;

    case WM_INITDIALOG:
        SetDlgItemTextA(hWnd, IDC_EDIT1, "C:\\Users\\example.txt");
        SetDlgItemTextA(hWnd, IDC_EDIT3, "C:\\Users\\copy.txt");
        return TRUE;

    case WM_COMMAND:
        if (wParam == IDC_BUTTON1) {
            StartCopying(hWnd);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
