#include <windows.h>
#include <tchar.h>
#include "resource.h"

HWND hList, hCombo, hCheck, hEdit;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc); 
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		return TRUE;

	case WM_INITDIALOG:
		hCombo = GetDlgItem(hWnd, IDC_COMBO1);
		SendMessage(hCombo, CB_RESETCONTENT, 0, 0);

		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Безадри-221"));
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Безадри-231"));
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Безадри-232"));
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Безадри-241"));
		SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)TEXT("Безадри-242"));

		SendMessage(hCombo, CB_SETCURSEL, 0, 0);
		return TRUE;

	case WM_COMMAND:
		if (wParam == IDC_BUTTON1) {
			EndDialog(hWnd, 0);
			return 1;
		}
	}
	return FALSE;
}