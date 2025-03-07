#ifndef ethernetinterfaceh
#define ethernetinterfaceh

#include <stdint.h>
#include <WinSock2.h>
#include <stdio.h>
#include <winsock.h>
#include <WS2tcpip.h>

class UDPClient {
    public:
        UDPClient() {
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                printf("WSAStartup failed\n");
            }
        };

        ~UDPClient() {
            if (clientSocket != INVALID_SOCKET) {
                closesocket(clientSocket);
            }
            WSACleanup();
        };

        void SetupClient(uint32_t port, const char* ip) {
            clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if (clientSocket == INVALID_SOCKET) {
                printf("Error creating client socket");
                WSACleanup();
                return;
            }

            // Configure server address
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);
            inet_pton(AF_INET, ip, &serverAddr.sin_addr);

            clientSetup = true;
        }

        void Send(const char* msg, size_t len) {
            if (!clientSetup) {
                printf("Client not setup");
                return;
            }
            if (sendto(clientSocket, msg, len, 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                printf("Error sending message: \"%s\"", msg);
                closesocket(clientSocket);
                WSACleanup();
                return;
            }
        }

        void Receive(char* buffer, size_t bufSize) {
            if (!clientSetup) {
                printf("Client not setup");
                return;
            }

            int serverAddrLen = sizeof(serverAddr);
            int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&serverAddr, &serverAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                printf("Error receiving message");
            } else {
                buffer[bytesReceived] = '\0';
                printf("Received message: \"%s\"\n", buffer);
            }
        }


    private:
        WSADATA wsaData;
        SOCKET clientSocket;
        sockaddr_in serverAddr;
        bool clientSetup = false;
};

class UDPServer {
    public:
        UDPServer() {
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                printf("WSAStartup failed\n");
            }
        };

        ~UDPServer() {
            if (serverSocket != INVALID_SOCKET) {
                closesocket(serverSocket);
            }
            WSACleanup();
        };

        void SetupServer(uint32_t port) {
            // Create socket
            serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if (serverSocket == INVALID_SOCKET) {
                printf("Error creating server socket");
                WSACleanup();
                return;
            }

            sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);
            serverAddr.sin_addr.s_addr = INADDR_ANY;

            // Bind socket
            if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                printf("Error binding server socket");
                closesocket(serverSocket);
                WSACleanup();
                return;
            }

            serverSetup = true;
        }

        void Send(const char* msg, size_t len) {
            if (!serverSetup) {
                printf("Server not setup");
                return;
            }
            if (sendto(serverSocket, msg, len, 0, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
                printf("Error sending message: \"%s\"", msg);
                closesocket(serverSocket);
                WSACleanup();
                return;
            }
        }

        size_t Receive(char* buffer, size_t bufSize) {
            if (!serverSetup) {
                printf("Server not setup");
                return 0;
            }

            int clientAddrLen = sizeof(clientAddr);
            int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&clientAddr, &clientAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                printf("Error receiving message");
            } else {
                buffer[bytesReceived] = '\0';
                if (bytesReceived > 0) {
                    printf("Received message: \"%s\"\n", buffer);
                }
            }
            return bytesReceived;
        }

    private:
        WSADATA wsaData;
        SOCKET serverSocket;
        sockaddr_in clientAddr;
        bool serverSetup = false;
};

#endif