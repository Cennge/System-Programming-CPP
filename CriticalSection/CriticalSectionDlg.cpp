#include "CriticalSectionDlg.h"
#include <fstream>
#include <cstdio>

using namespace std;

CriticalSectionDlg* CriticalSectionDlg::ptr = NULL;
CRITICAL_SECTION cs;
bool writeCompleted = false; // Флаг завершения записи

const int NUM_COPIES = 5;

CriticalSectionDlg::CriticalSectionDlg(void)
{
    ptr = this;
}

CriticalSectionDlg::~CriticalSectionDlg(void)
{
    DeleteCriticalSection(&cs);
}

void CriticalSectionDlg::Cls_OnClose(HWND hwnd)
{
    EndDialog(hwnd, 0);
}

BOOL CriticalSectionDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    InitializeCriticalSection(&cs);
    return TRUE;
}

void MessageAboutError(DWORD dwError)
{
    LPVOID lpMsgBuf = NULL;
    TCHAR szBuf[300];
    BOOL fOK = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
    if (lpMsgBuf != NULL)
    {
        wsprintf(szBuf, TEXT("Ошибка %d: %s"), dwError, lpMsgBuf);
        MessageBox(0, szBuf, TEXT("Сообщение об ошибке"), MB_OK | MB_ICONSTOP);
        LocalFree(lpMsgBuf);
    }
}

DWORD WINAPI WriteToFiles(LPVOID lp)
{
    EnterCriticalSection(&cs);
    ifstream src("source.txt", ios::binary);
    if (!src.is_open())
    {
        MessageAboutError(GetLastError());
        LeaveCriticalSection(&cs);
        return 1;
    }

    char filename[20];
    for (int i = 1; i <= NUM_COPIES; i++)
    {
        sprintf(filename, "copy%d.txt", i);
        ofstream out(filename, ios::binary);
        if (!out.is_open())
        {
            MessageAboutError(GetLastError());
            LeaveCriticalSection(&cs);
            return 1;
        }
        out << src.rdbuf();
        out.close();
        src.clear();
        src.seekg(0, ios::beg);
    }
    src.close();

    writeCompleted = true; // Флаг завершения записи
    LeaveCriticalSection(&cs);
    MessageBox(0, TEXT("Файлы-копии созданы"), TEXT("Критическая секция"), MB_OK);
    return 0;
}

DWORD WINAPI ReadFromFiles(LPVOID lp)
{
    EnterCriticalSection(&cs);

    // Ждем завершения записи, если флаг не установлен
    while (!writeCompleted)
    {
        LeaveCriticalSection(&cs);
        Sleep(100); // Даём потоку записи возможность закончить
        EnterCriticalSection(&cs);
    }

    ofstream result("result.txt", ios::binary);
    if (!result.is_open())
    {
        MessageAboutError(GetLastError());
        LeaveCriticalSection(&cs);
        return 1;
    }

    char filename[20];
    for (int i = 1; i <= NUM_COPIES; i++)
    {
        sprintf(filename, "copy%d.txt", i);
        ifstream in(filename, ios::binary);
        if (!in.is_open())
        {
            MessageAboutError(GetLastError());
            LeaveCriticalSection(&cs);
            return 1;
        }
        result << in.rdbuf();
        in.close();
    }
    result.close();
    LeaveCriticalSection(&cs);
    MessageBox(0, TEXT("Результирующий файл сформирован"), TEXT("Критическая секция"), MB_OK);
    return 0;
}

void CriticalSectionDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    if (id == IDC_BUTTON1)
    {
        HANDLE hWriteThread = CreateThread(NULL, 0, WriteToFiles, 0, 0, NULL);
        HANDLE hReadThread = CreateThread(NULL, 0, ReadFromFiles, 0, 0, NULL);

        WaitForSingleObject(hWriteThread, INFINITE);
        WaitForSingleObject(hReadThread, INFINITE);

        CloseHandle(hWriteThread);
        CloseHandle(hReadThread);
    }
}

BOOL CALLBACK CriticalSectionDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
        HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
    }
    return FALSE;
}
