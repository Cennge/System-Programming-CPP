#include "framework.h"
#include "blablabla.h"

#define MAX_LOADSTRING 100
#define RESUME_BUTTON 1

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

HWND hButton;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int AskUser(int guess) {
    wchar_t buffer[50];
    swprintf(buffer, 50, L"Ваше число %d?", guess);

    int result = MessageBox(NULL, buffer, L"Угадай число", MB_YESNOCANCEL | MB_ICONQUESTION);
    if (result == IDYES) return 0;
    if (result == IDCANCEL) return -2;  // Если пользователь нажал "Отмена" — выход из игры

    result = MessageBox(NULL, L"Ваше число больше?", L"Угадай число", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES) return 1;

    return -1;  // Если нажали "Нет", значит число меньше
}

void PlayGame() {
    int low = 1, high = 100, attempts = 0;
    while (low <= high) {
        int mid = (low + high) / 2;
        int response = AskUser(mid);
        attempts++;

        if (response == 0) {
            wchar_t buffer[50];
            swprintf(buffer, 50, L"Число угадано за %d попыток!", attempts);
            MessageBox(NULL, buffer, L"Победа!", MB_OK | MB_ICONINFORMATION);
            break;
        }
        else if (response == 1) {
            low = mid + 1;
        }
        else if (response == -1) {
            high = mid - 1;
        }
        else {
            return;  // Если нажали "Отмена", выходим из игры
        }
    }

    int retry = MessageBox(NULL, L"Хотите сыграть еще раз?", L"Новая игра", MB_YESNO | MB_ICONQUESTION);
    if (retry == IDYES) PlayGame();
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    PlayGame();
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BLABLABLA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BLABLABLA));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLABLABLA));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BLABLABLA);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    TCHAR str[50];

    switch (uMessage)
    {
    case WM_CREATE:
        hButton = CreateWindowW(
            L"BUTTON",
            L"Резюме!",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 50, 150, 40,
            hWnd,
            (HMENU)RESUME_BUTTON,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL
        );
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == RESUME_BUTTON)
        {
            const wchar_t* text1 = L"Написать приложение, позволяющее вывести на экран краткое резюме с помощью последовательности окон сообщений";
            const wchar_t* text2 = L"(количество окон сообщений – не менее трёх). На заголовке последнего окна сообщения должно отобразиться";
            const wchar_t* text3 = L"среднее число символов на странице (общее число символов в резюме поделить на количество окон сообщений)";

            double d_title = (wcslen(text1) + wcslen(text2) + wcslen(text3)) / 3.0;

            const int size = 64;
            wchar_t title[size];
            swprintf(title, size, L"%.2f", d_title);

            MessageBox(hWnd, text1, L"Резюме", MB_OK | MB_ICONINFORMATION);
            MessageBox(hWnd, text2, L"Резюме", MB_OK | MB_ICONINFORMATION);
            MessageBox(hWnd, text3, title, MB_OK | MB_ICONINFORMATION);
        }
        break;

    case WM_LBUTTONDOWN:
        wsprintf(str, TEXT("Hello world!"));
        SetWindowText(hWnd, str);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, uMessage, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}