#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "include/TcpServer.h"
#include "include/BroadcastServer.h"
#pragma comment(lib, "ws2_32.lib")

int main(){
    TcpServer server;
    BroadcastServer broadcast;
    bool endBroadcast = false;

    if (server.initialize())
    {
        std::cout << "TCP server initialized!" << std::endl;
        std::cout << "Listening on port: " << server.port() << std::endl;
    }
    if (broadcast.initialize())
    {
        std::cout << "Broadcast initialized!" << std::endl;
        std::cout << "Broadcasting on port: " << broadcast.port() << std::endl;
    }

    // Starting a thread to do broadcast
    std::thread broadcastThread([&endBroadcast, &broadcast, &server](){
        while (!endBroadcast)
        {
            if (!server.hasClient())
            {
                std::cout << "Thread :: Broadcasting server port..." << std::endl;
                broadcast.broadcastServerPort();
            }
            Sleep(2000);
        }
        return;
    });

    // Now wait if anyone wants to connect:
    server.blockUntilNextClient();
    std::cout << "Active client found!" << std::endl;

    //chat with client
    const std::string shutDownCmd = "shut";
    bool state = true;
    while(state){
        char buffer[200];
        int bytesReceived = server.receiveBytes(buffer, sizeof(buffer));
        std::string cmd;
        if(bytesReceived > 0)
        {
            cmd = std::string(buffer).substr(0, bytesReceived);
            std::cout << "Message received: "<< cmd << std::endl;
        }

        if(cmd == shutDownCmd){
            state = false;
            std::cout << "Disconnecting current client and waiting for a new client..." << std::endl;
            server.blockUntilNextClient(WaitClientMode::DisconnectActiveClient);
            state = true;
            continue;
        }
        
        char confirmation[200];
        std::cout << "Please enter your message to client: ";
        std::cin.getline(confirmation, sizeof(confirmation));
        int bytesSent = server.sendBytes(confirmation, sizeof(confirmation));
        
        state = confirmation == shutDownCmd;
    }
    
    endBroadcast = true;
    broadcastThread.join();
    system("pause");
    return 0;
}