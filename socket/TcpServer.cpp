#include "TcpServer.h"
#include <iostream>

TcpServer::TcpServer(int port) : m_port(port)
{
    WSADATA wsaData;
    int wsaError;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaError = WSAStartup(wVersionRequested, &wsaData);
    m_isWsaStarted = wsaError == 0;
    if (wsaError != 0)
    {
        std::cout << "TcpServer :: Could not initialize WSA, status: " << wsaData.szSystemStatus << std::endl;
    }
}

TcpServer::~TcpServer()
{
    if (m_isWsaStarted)
    {
        close();
        WSACleanup();
    }
}

bool TcpServer::initialize()
{
    if (!m_isWsaStarted)
        return false;

    bool ok = true;
    //creating server socket
    m_activeClient = INVALID_SOCKET;
    m_serverSocket = INVALID_SOCKET;
    m_serverSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
    ok = m_serverSocket != INVALID_SOCKET;
    if (m_serverSocket == INVALID_SOCKET){
        std::cout << "Error at socket() :"<<WSAGetLastError() <<std::endl;
    }

    if (ok)
    {
        //bind the socket to port
        sockaddr_in service;
        service.sin_family = AF_INET;
        //Inet_Pton is somehow not available
        service.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");//
        service.sin_port = htons(m_port);
        ok = bind(m_serverSocket, (SOCKADDR*)&service, sizeof(service)) == 0;
    }
    
    if (ok)
    {
        ok = listen(m_serverSocket, 1) == 0;
    }

    return ok;
}

void TcpServer::blockUntilNextClient(WaitClientMode mode)
{
    if (!hasClient() || mode == WaitClientMode::DisconnectActiveClient)
    {
        if (m_activeClient != INVALID_SOCKET)
        {
            closesocket(m_activeClient);
            m_activeClient = INVALID_SOCKET;
        }
        m_activeClient = accept(m_serverSocket,NULL,NULL);
        if (m_activeClient == INVALID_SOCKET)
        {
            std::cout << "accept failed :" << WSAGetLastError() << std::endl;
        }
    }
}

int TcpServer::receiveBytes(char* buffer, int size)
{
    if (!hasClient())
        return -1;
    return recv(m_activeClient, buffer, size, 0);
}

int TcpServer::sendBytes(char* buffer, int size)
{
    if (!hasClient())
        return -1;
    return send(m_activeClient, buffer, size, 0);
}

void TcpServer::close()
{
    closesocket(m_serverSocket);
    closesocket(m_activeClient);
}