#include <iostream>
#include <tchar.h>

using namespace std;

bool isPalindrome(const _TCHAR* str) {
    int len = _tcslen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) return false;
    }
    return true;
}

int countWords(const _TCHAR* str) {
    int count = 0;
    bool inWord = false;

    for (int i = 0; str[i] != _T('\0'); i++) {
        if (str[i] != _T(' ')) {
            if (!inWord) {
                count++;
                inWord = true;
            }
        }
        else {
            inWord = false;
        }
    }

    return count;
}

int main() {
    setlocale(LC_ALL, "");
    _TCHAR szBuf3[100];

    wcout << L"Введите строку: " << endl;
    wcin.getline(szBuf3, 100);

    if (isPalindrome(szBuf3)) wcout << L"Палиндром." << endl;
    else wcout << L"Не палиндром" << endl;

    int wordCount = countWords(szBuf3);
    wcout << L"Количество слов: " << wordCount << endl;

    return 0;
}