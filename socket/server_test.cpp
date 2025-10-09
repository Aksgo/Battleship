#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "include/TcpServer.h"
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(){
    TcpServer server;
    if (server.initialize())
    {
        std::cout << "TCP server initialized!" << std::endl;
        std::cout << "Listening on port: " << server.port() << std::endl;
    }
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
        cin.getline(confirmation, sizeof(confirmation));
        int bytesSent = server.sendBytes(confirmation, sizeof(confirmation));
        
        state = confirmation == shutDownCmd;
    }
    system("pause");
    return 0;
}