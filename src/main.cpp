#include <cstdio>
#include <chrono>
#include "EthernetInterface.h"
#include <thread>

int main(int argc, char *argv[])
{
    UDPClient client;
    client.SetupClient(2000, "127.0.0.1");

    UDPServer server;
    server.SetupServer(2000);

    std::thread thread([&client] {
        printf("Send Thread Started\n");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        client.Send("Test", 5);
        printf("Send Thread Ended\n");
    });

    char buffer[1024];
    int rec = 0;
    while (rec == 0) {
        rec = server.Receive(buffer, sizeof(buffer));
    }

    thread.join();
    return 0;
}
