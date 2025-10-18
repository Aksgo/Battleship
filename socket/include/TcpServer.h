#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>

enum class WaitClientMode { Normal, DisconnectActiveClient };

class TcpServer
{
public:
    TcpServer(int port = 55555);
    virtual ~TcpServer();

    bool initialize();
    void blockUntilNextClient(WaitClientMode mode = WaitClientMode::Normal);
    int receiveBytes(char* buffer, int size);
    int sendBytes(char* buffer, int size);
    void close();

    int port() const { return m_port; }
    bool hasClient() const { return m_activeClient != INVALID_SOCKET; }

private:
    bool m_isWsaStarted = false;
    int m_port;
    SOCKET m_serverSocket;
    SOCKET m_activeClient;
};