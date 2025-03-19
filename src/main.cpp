#include <cstdio>
#include <chrono>
#include "EthernetInterface.h"
#include <thread>

int main(int argc, char *argv[])
{
    UDPMulticastClient client;
    client.SetupClient(2002, "234.56.78.90");

    UDPMulticastServer server;
    server.SetupServer(2002, "234.56.78.90", true);

    if (!server.IsSetup()) {
        printf("[Server] Setup failed\n");
        return 1;
    } else if (!client.IsSetup()) {
        printf("[Client] Setup failed\n");
        return 1;
    }

    char buffer[1024];
    int rec = 0;

    std::thread thread([&](){
        char buffer[1024];
        printf("[Client] Thread Started\n");
        std::this_thread::sleep_for(std::chrono::seconds(2));
        printf("[Client] Sending message\n");
        client.Send("Message", 8);
        while(!rec) {}
        rec = 0;
        while(!rec) {
            rec = client.Receive(buffer, sizeof(buffer));
            if (rec > 0) {
                printf("[Client] Received message: \"%s\"\n", buffer);
            }
        }
        printf("[Client] Thread Ended\n");
    });

    printf("[Server] Waiting for message\n");
    while (!rec) {
        rec = server.Receive(buffer, sizeof(buffer));
        if (rec > 0) {
            printf("[Server] Received message: \"%s\"\n", buffer);
            break;
        }
    }
    printf("[Server] Sending reply\n");
    server.Send("Reply", 6);

    thread.join();
    printf("[Server] Thread Ended\n");
    return 0;

    // UDPClient client;
    // client.SetupClient(2000, "127.0.0.1");

    // UDPServer server;
    // server.SetupServer(2000);

    // char buffer[1024];
    // int rec = 0;

    // std::thread thread([&](){
    //     char buffer[1024];
    //     printf("[Client] Thread Started\n");
    //     std::this_thread::sleep_for(std::chrono::seconds(2));
    //     printf("[Client] Sending message\n");
    //     client.Send("Message", 8);
    //     while(!rec) {}
    //     rec = 0;
    //     while(!rec) {
    //         rec = client.Receive(buffer, sizeof(buffer));
    //         if (rec > 0) {
    //             printf("[Client] Received message: \"%s\"\n", buffer);
    //         }
    //     }
    //     printf("[Client] Thread Ended\n");
    // });

    // printf("[Server] Waiting for message\n");
    // while (!rec) {
    //     rec = server.Receive(buffer, sizeof(buffer));
    //     if (rec > 0) {
    //         printf("[Server] Received message: \"%s\"\n", buffer);
    //         break;
    //     }
    // }
    // printf("[Server] Sending reply\n");
    // server.Send("Reply", 6);

    // thread.join();
    // printf("[Server] Thread Ended\n");
    // return 0;
}
