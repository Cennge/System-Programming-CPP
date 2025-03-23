#include <winsock2.h>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

#define MAX_CLIENTS 10
#define DEFAULT_BUFLEN 4096

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

SOCKET server_socket;

time_t client_connection_times[MAX_CLIENTS] = { 0 };

vector<string> history;

int main() {
    system("title Server");

    puts("Start server... DONE.");
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
        return 2;
    }

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d", WSAGetLastError());
        return 3;
    }

    listen(server_socket, MAX_CLIENTS);

    puts("Server is waiting for incoming connections...\nPlease, start one or more client-side app.");

    fd_set readfds;
    SOCKET client_socket[MAX_CLIENTS] = { 0 };

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            SOCKET s = client_socket[i];
            if (s > 0) {
                FD_SET(s, &readfds);
            }
        }

        if (select(0, &readfds, NULL, NULL, NULL) == SOCKET_ERROR) {
            printf("select function call failed with error code : %d", WSAGetLastError());
            return 4;
        }

        SOCKET new_socket;
        sockaddr_in address;
        int addrlen = sizeof(sockaddr_in);
        if (FD_ISSET(server_socket, &readfds)) {
            if ((new_socket = accept(server_socket, (sockaddr*)&address, &addrlen)) < 0) {
                perror("accept function error");
                return 5;
            }

            for (const auto& message : history) {
                send(new_socket, message.c_str(), message.size(), 0);
            }

            printf("New connection, socket fd is %d, ip is: %s, port: %d\n", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    client_connection_times[i] = time(nullptr);
                    printf("Adding to list of sockets at index %d\n", i);
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            SOCKET s = client_socket[i];
            if (FD_ISSET(s, &readfds)) {
                getpeername(s, (sockaddr*)&address, (int*)&addrlen);

                char client_message[DEFAULT_BUFLEN];
                int client_message_length = recv(s, client_message, DEFAULT_BUFLEN, 0);
                client_message[client_message_length] = '\0';

                string check_exit = client_message;
                if (check_exit == "off") {
                    time_t disconnect_time = time(nullptr);
                    time_t connect_time = client_connection_times[i];
                    double duration = difftime(disconnect_time, connect_time);

                    string disconnect_msg = "You were connected for " + to_string(static_cast<int>(duration));
                    send(s, disconnect_msg.c_str(), disconnect_msg.size(), 0);

                    cout << "Client #" << i << " disconnected connection duration: " << static_cast<int>(duration);

                    closesocket(s);
                    client_socket[i] = 0;
                    client_connection_times[i] = 0;
                    continue;
                }

                string temp = client_message;
                history.push_back(temp);

                for (int j = 0; j < MAX_CLIENTS; j++) {
                    if (client_socket[j] != 0) {
                        send(client_socket[j], client_message, client_message_length, 0);
                    }
                }
            }
        }
    }

    WSACleanup();
}
