#include <iostream>
#include "Socket.h"
#include <cstring>

using namespace std;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int nChoice;
    int port = 24242; // выбираем порт
    string ipAddress = "127.0.0.1"; // Адрес сервера

    char receiveMessage[MAXSTRLEN];
    char sendMessage[MAXSTRLEN];

    int order[4] = { 0 }; // Массив для хранения количества заказанных позиций
    int prices[4] = { 0, 30, 40, 25 }; // Цены на товары (индекс соответствует позиции)

    cout << "Выберите действие:" << endl;
    cout << "1 - Запустить сервер" << endl;
    cout << "2 - Подключиться к серверу" << endl;
    cout << "3 - Выйти" << endl;
    cin >> nChoice;
    cin.ignore(); // очистка буфера ввода

    if (nChoice == 1)
    {
        ServerSocket server;
        cout << "Запуск сервера..." << endl;
        server.StartHosting(port);

        while (true)
        {
            cout << "Ожидание данных..." << endl;
            server.ReceiveData(receiveMessage, MAXSTRLEN);
            cout << "Получено сообщение: " << receiveMessage << endl;

            if (strcmp(receiveMessage, "Menu") == 0)
            {
                string menu = "1. Кола (30 грн)\n2. Картошка фри (40 грн)\n3. Мороженое (25 грн)\n5. Чек\n";
                server.SendDataMessage(menu.c_str());
                continue;
            }

            if (strcmp(receiveMessage, "5") == 0)
            {
                int total = 0;
                string receipt = "Ваш заказ:\n";
                for (int i = 1; i <= 3; i++)
                {
                    if (order[i] > 0)
                    {
                        receipt += "Позиция " + to_string(i) + " | Количество: " + to_string(order[i]) + " | Сумма: " + to_string(order[i] * prices[i]) + " грн\n";
                        total += order[i] * prices[i];
                    }
                }
                receipt += "Общая сумма: " + to_string(total) + " грн\n";
                server.SendDataMessage(receipt.c_str());
                continue;
            }

            // Разбираем полученные данные в формате номерпозиции-количество
            string receivedText(receiveMessage);
            size_t separatorPos = receivedText.find('-');
            if (separatorPos != string::npos) {
                string itemIndexStr = receivedText.substr(0, separatorPos);
                string itemCountStr = receivedText.substr(separatorPos + 1);

                int itemIndex = stoi(itemIndexStr);
                int itemCount = stoi(itemCountStr);

                if (itemIndex >= 1 && itemIndex <= 3 && itemCount > 0) {
                    order[itemIndex] += itemCount;
                    string response = "Заказ принят: позиция " + to_string(itemIndex) + ", количество " + to_string(itemCount);
                    server.SendDataMessage(response.c_str());
                }
                else {
                    server.SendDataMessage("Ошибка: неверный формат заказа");
                }
            }
            else {
                server.SendDataMessage("Ошибка: неверный формат заказа");
            }

            if (strcmp(receiveMessage, "end") == 0)
                break;
        }
    }
    else if (nChoice == 2)
    {
        cout << "Подключение к серверу по адресу " << ipAddress << "..." << endl;
        ClientSocket client;
        client.ConnectToServer(ipAddress.c_str(), port);

        while (true)
        {
            cout << "Введите заказ (номер-количество) или '5' для расчета: ";
            string input;
            getline(cin, input);
            client.SendDataMessage(input.c_str());

            cout << "Ожидание ответа..." << endl;
            client.ReceiveData(receiveMessage, MAXSTRLEN);
            cout << "Ответ сервера: " << receiveMessage << endl;

            if (input == "end" || strcmp(receiveMessage, "end") == 0)
                break;
        }
        client.CloseConnection();
    }
    else if (nChoice == 3)
    {
        cout << "Завершение работы..." << endl;
        return 0;
    }
}