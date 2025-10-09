#include "BroadcastServer.h"
#include <iostream>

BroadcastServer::BroadcastServer(std::string ip, int port)
    : m_ip(ip), m_port(port)
{
    WSADATA wsaData;
    int wsaError;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaError = WSAStartup(wVersionRequested, &wsaData);
    m_isWsaStarted = wsaError == 0;
    if (wsaError != 0)
    {
        std::cout << "BroadcastServer :: Could not initialize WSA, status: " << wsaData.szSystemStatus << std::endl;
    }
}

BroadcastServer::~BroadcastServer()
{
    if (m_isWsaStarted)
    {
        close();
        WSACleanup();
    }
}

bool BroadcastServer::initialize()
{
    if (!m_isWsaStarted)
        return false;

    bool ok = true;
    
    m_serverSocket = INVALID_SOCKET;
    m_serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    m_broadcast.sin_family = AF_INET;
    m_broadcast.sin_port = htons(m_port);
    m_broadcast.sin_addr.s_addr = inet_addr(m_ip.c_str());
    BOOL broadcastEnable = TRUE;
    setsockopt(m_serverSocket, SOL_SOCKET, SO_BROADCAST,(char*)&broadcastEnable, sizeof(broadcastEnable));

    return ok;
}

void BroadcastServer::broadcastServerPort(int serverPort) const
{
    if (!m_isWsaStarted)
        return;

    std::string msg = "SERVER:" + std::to_string(serverPort);
    sendto(m_serverSocket, msg.c_str(), msg.size(), 0, (struct sockaddr*)&m_broadcast, sizeof(m_broadcast));
}

void BroadcastServer::close()
{
    closesocket(m_serverSocket);
}