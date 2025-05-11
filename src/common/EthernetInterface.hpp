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
                printf("[Client] WSAStartup failed\n");
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
                printf("[Client] Error creating client socket\n");
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
                printf("[Client] Not setup\n");
                return;
            }
            if (sendto(clientSocket, msg, len, 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                printf("[Client] Error sending message: \"%s\"\n", msg);
                closesocket(clientSocket);
                WSACleanup();
                return;
            }
        }

        size_t Receive(char* buffer, size_t bufSize) {
            if (!clientSetup) {
                printf("[Client] Not setup\n");
                return 0;
            }

            int serverAddrLen = sizeof(serverAddr);
            int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddr, &serverAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                printf("[Client] Error receiving message\n");
            } else {
                buffer[bytesReceived] = '\0';
            }
            return bytesReceived;
        }

        bool IsSetup() {
            return clientSetup;
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
                printf("[Server] WSAStartup failed\n");
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
                printf("[Server] Error creating server socket\n");
                WSACleanup();
                return;
            }

            sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);
            serverAddr.sin_addr.s_addr = INADDR_ANY;

            // Bind socket
            if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                printf("[Server] Error binding server socket\n");
                closesocket(serverSocket);
                WSACleanup();
                return;
            }

            serverSetup = true;
        }

        void Send(const char* msg, size_t len) {
            if (!serverSetup) {
                printf("[Server] Not setup\n");
                return;
            }
            if (sendto(serverSocket, msg, len, 0, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
                printf("[Server] Error sending message: \"%s\"\n", msg);
                printf("[Server] Error: %d\n", WSAGetLastError());
                closesocket(serverSocket);
                WSACleanup();
                return;
            }
        }

        size_t Receive(char* buffer, size_t bufSize) {
            if (!serverSetup) {
                printf("[Server] Not setup\n");
                return 0;
            }

            int clientAddrLen = sizeof(clientAddr);
            int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                printf("[Server] Error receiving message\n");
                printf("[Server] Error: %d\n", WSAGetLastError());
                return 0;
            } else {
                buffer[bytesReceived] = '\0';
            }
            return bytesReceived;
        }

        bool IsSetup() {
            return serverSetup;
        }

    private:
        WSADATA wsaData;
        SOCKET serverSocket;
        sockaddr_in clientAddr;
        bool serverSetup = false;
};

class UDPMulticastClient {
    public:
        UDPMulticastClient() {
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                printf("[Client] WSAStartup failed\n");
            }
        };

        ~UDPMulticastClient() {
            if (clientSocket != INVALID_SOCKET) {
                closesocket(clientSocket);
            }
            WSACleanup();
        };

        void SetupClient(uint32_t port, const char* ip) {
            clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if (clientSocket == INVALID_SOCKET) {
                printf("[Client] Error creating client socket\n");
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
                printf("[Client] Not setup\n");
                return;
            }
            if (sendto(clientSocket, msg, len, 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                printf("[Client] Error sending message: \"%s\"\n", msg);
                closesocket(clientSocket);
                WSACleanup();
                return;
            }
        }

        size_t Receive(char* buffer, size_t bufSize) {
            if (!clientSetup) {
                printf("[Client] Not setup\n");
                return 0;
            }

            int serverAddrLen = sizeof(serverAddr);
            int bytesReceived = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddr, &serverAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                printf("[Client] Error receiving message\n");
                return 0;
            } else {
                buffer[bytesReceived] = '\0';
            }
            return bytesReceived;
        }

        bool IsSetup() {
            return clientSetup;
        }

    private:
        WSADATA wsaData;
        SOCKET clientSocket;
        sockaddr_in serverAddr;
        bool clientSetup = false;
};

class UDPMulticastServer {
    public:
        UDPMulticastServer() {
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                printf("[Server] WSAStartup failed\n");
            }
        };

        ~UDPMulticastServer() {
            if (serverSocket != INVALID_SOCKET) {
                closesocket(serverSocket);
            }
            WSACleanup();
        };

        void SetupServer(uint32_t port, const char* ip, bool internal = false) {
            // Create socket
            serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
            if (serverSocket == INVALID_SOCKET) {
                printf("[Server] Error creating server socket\n");
                WSACleanup();
                return;
            }

            // Allow reuse of port
            bool yes = 1;
            if ((setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes))) < 0)
            {
                printf("[Server] Error setting socket option: REUSEADDR\n");
                closesocket(serverSocket);
                return;
            }

            sockaddr_in serverAddr;
            memset(&serverAddr, 0, sizeof(serverAddr));
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(port);
            serverAddr.sin_addr.s_addr = INADDR_ANY;

            // Bind socket
            if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                printf("[Server] Error binding server socket\n");
                closesocket(serverSocket);
                WSACleanup();
                return;
            }
            
            ip_mreq mreq;
            memset(&mreq, 0, sizeof(mreq));

            if (internal) {
                yes = true;
                if (setsockopt(serverSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char*)&yes, sizeof(yes)) == SOCKET_ERROR) {
                    printf("[Server] Error setting socket option: MULTICAST LOOP\n");
                    printf("[Server] Error: %d\n", WSAGetLastError());
                    closesocket(serverSocket);
                    return;
                }
                inet_pton(AF_INET, "30.30.30.3", &mreq.imr_interface);
            } else {
                // mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            }
            // mreq.imr_interface.s_addr = htonl(INADDR_ANY);

            if (inet_pton(AF_INET, ip, &mreq.imr_multiaddr.s_addr) < 0) {
                printf("[Server] Error setting multicast address\n");
                closesocket(serverSocket);
                return;
            }
        
            if (setsockopt(serverSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) == SOCKET_ERROR) {
                printf("[Server] Error setting socket option: MULTICAST MEMBERSHIP\n");
                printf("[Server] Error: %d\n", WSAGetLastError());
                closesocket(serverSocket);
                return;
            }

            serverSetup = true;
        }

        void Send(const char* msg, size_t len) {
            if (!serverSetup) {
                printf("[Server] Not setup\n");
                return;
            }
            if (sendto(serverSocket, msg, len, 0, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
                printf("[Server] Error sending message: \"%s\"\n", msg);
                printf("[Server] Error: %d\n", WSAGetLastError());
                closesocket(serverSocket);
                WSACleanup();
                return;
            }
        }

        size_t Receive(char* buffer, size_t bufSize) {
            if (!serverSetup) {
                printf("[Server] Not setup\n");
                return 0;
            }

            int clientAddrLen = sizeof(clientAddr);
            int bytesReceived = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
            if (bytesReceived == SOCKET_ERROR) {
                printf("[Server] Error receiving message\n");
                printf("[Server] Error: %d\n", WSAGetLastError());
            } else {
                buffer[bytesReceived] = '\0';
            }
            return bytesReceived;
        }

        bool IsSetup() {
            return serverSetup;
        }

    private:
        WSADATA wsaData;
        SOCKET serverSocket;
        sockaddr_in clientAddr;
        bool serverSetup = false;
};

#endif