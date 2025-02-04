#define _CRT_SECURE_NO_WARNINGS
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

void searchAndReplace(_TCHAR* text, _TCHAR* find, _TCHAR* replace) {
	_TCHAR* ind = _tcsstr(text, find);
	if (!ind) return;

	_TCHAR newText[50] = _TEXT("");
	_tcsncat(newText, text, ind - text);
	_tcscat(newText, replace);
	_tcscat(newText, ind + _tcslen(find));

	_tcscpy(text, newText);
	wcout << text << endl;
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

	_TCHAR text[50] = _TEXT("Hello, world!");
	_TCHAR find[] = _TEXT("Hello");
	_TCHAR replace[] = _TEXT("bye bye");

	searchAndReplace(text, find, replace);

	system("pause");
}