#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

class BroadcastServer
{
public:
    BroadcastServer(std::string ip = "127.0.0.1", int port = 60000);
    virtual ~BroadcastServer();

    bool initialize();
    void broadcastServerPort(int serverPort = 55555) const;
    void close();
    
    int port() const { return m_port; }

private:
    bool m_isWsaStarted = false;
    const std::string m_ip;
    int m_port;
    
    SOCKET m_serverSocket;
    sockaddr_in m_broadcast;
};