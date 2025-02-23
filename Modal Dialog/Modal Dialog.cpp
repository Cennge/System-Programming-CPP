#include <windows.h>
#include <tchar.h>
#include <string>
#include "resource.h"

using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void CreateButton(HWND hWnd, int x, int y, int width, int height, int id, LPCWSTR text);
void UpdateButtonText(HWND hWnd, int id);
bool CheckWinner();
void ResetGame(HWND hWnd);

bool playerX = true;
int moveCount = 0;
wstring buttonText[9] = { L"-", L"-", L"-", L"-", L"-", L"-", L"-", L"-", L"-" };

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        CreateButton(hWnd, 50, 50, 100, 100, 1, L"-");
        CreateButton(hWnd, 50, 170, 100, 100, 2, L"-");
        CreateButton(hWnd, 50, 290, 100, 100, 3, L"-");
        CreateButton(hWnd, 170, 50, 100, 100, 4, L"-");
        CreateButton(hWnd, 170, 170, 100, 100, 5, L"-");
        CreateButton(hWnd, 170, 290, 100, 100, 6, L"-");
        CreateButton(hWnd, 290, 50, 100, 100, 7, L"-");
        CreateButton(hWnd, 290, 170, 100, 100, 8, L"-");
        CreateButton(hWnd, 290, 290, 100, 100, 9, L"-");

        if (playerX) SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_SETCHECK, BST_CHECKED, 0);
        else SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_SETCHECK, BST_UNCHECKED, 0);

        return TRUE;

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);

        if (id >= 1 && id <= 9 && buttonText[id - 1] == L"-")
        {
            UpdateButtonText(hWnd, id - 1);
            moveCount++;

            if (CheckWinner()) {
                MessageBox(hWnd, playerX ? L"X wins!" : L"O wins!", L"Game Over", MB_OK);
                ResetGame(hWnd);
            }
            if (moveCount == 9) {
                MessageBox(hWnd, L"Draw!", L"Game Over", MB_OK);
                ResetGame(hWnd);
            }
        }

        if (LOWORD(wParam) == IDC_BUTTON1) ResetGame(hWnd);

        if (moveCount > 0 && (id == IDC_CHECK1)) return TRUE;

        if (id == IDC_CHECK1) {
            if (SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_GETCHECK, 0, 0) == BST_CHECKED) playerX = true;
            else playerX = false;
        }

        return TRUE;
    }

    case WM_CLOSE:
        EndDialog(hWnd, 0);
        return TRUE;
    }
    return FALSE;
}

void CreateButton(HWND hWnd, int x, int y, int width, int height, int id, LPCWSTR text)
{
    CreateWindow(
        L"BUTTON", text, WS_CHILD | WS_VISIBLE | BS_CENTER | WS_BORDER,
        x, y, width, height, hWnd,
        (HMENU)id, GetModuleHandle(NULL), NULL);
}

void UpdateButtonText(HWND hWnd, int id)
{
    if (buttonText[id] != L"-") return;

    if (playerX) buttonText[id] = L"O";
    else buttonText[id] = L"X";
    playerX = !playerX;

    HWND hButton = GetDlgItem(hWnd, id + 1);
    SetWindowText(hButton, buttonText[id].c_str());
}

int winningCombinations[8][3] = {
    { 0, 1, 2 },
    { 3, 4, 5 },
    { 6, 7, 8 },
    { 0, 3, 6 },
    { 1, 4, 7 },
    { 2, 5, 8 },
    { 0, 4, 8 },
    { 2, 4, 6 }
};

bool CheckWinner()
{
    for (int i = 0; i < 8; i++) {
        int* combo = winningCombinations[i];

        if (buttonText[combo[0]] != L"-" &&
            buttonText[combo[0]] == buttonText[combo[1]] &&
            buttonText[combo[0]] == buttonText[combo[2]])
        {
            return true;
        }
    }
    return false;
}

void ResetGame(HWND hWnd) {
    for (int i = 0; i < 9; i++) {
        buttonText[i] = L"-";
        HWND hButton = GetDlgItem(hWnd, i + 1);
        SetWindowText(hButton, L"-");
    }

    if (SendMessage(GetDlgItem(hWnd, IDC_CHECK1), BM_GETCHECK, 0, 0) == BST_CHECKED) playerX = true;
    else playerX = false;

    moveCount = 0;
}
