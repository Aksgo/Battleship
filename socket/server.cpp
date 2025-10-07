#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(){
    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    //loading dll file to create sockets
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr!=0){
        cout<<"Winsock dll not found"<<endl;
        return 0;
    }else{
        cout<<"Winsock dll found"<<endl;
        cout<<"The status : "<<wsaData.szSystemStatus<<endl;
    }

    //creating server socket
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
    if(serverSocket == INVALID_SOCKET){
        cout<<"Error at socket() :"<<WSAGetLastError()<<endl;
        WSACleanup();
        return 0;
    }else{
        cout<<"Socket() is OK"<<endl;
    }

    //bind the socket to port
    sockaddr_in service;
    service.sin_family = AF_INET;
    //Inet_Pton is somehow not available
    service.sin_addr.s_addr = INADDR_ANY; //inet_addr("127.0.0.1");
    service.sin_port = htons(port);
    if(bind(serverSocket, (SOCKADDR*)&service, sizeof(service))==SOCKET_ERROR){
        cout<<"bind() failed :"<<WSAGetLastError()<<endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }else{
        cout<<"bind() is OK"<<endl;
    }

    //opening socket to listen
    if(listen(serverSocket, 1) == SOCKET_ERROR){
        cout<<"listen() Error : "<<WSAGetLastError()<<endl;
    }else{
        cout<<"listen() is OK : waiting for connections..."<<endl;
    }

    //accept() the incoming clients
    acceptSocket = accept(serverSocket,NULL,NULL);
    if(acceptSocket == INVALID_SOCKET){
        cout<<"accept failed :"<<WSAGetLastError()<<endl;
        WSACleanup();
        return 1;
    }

    cout<<"Accepting connection"<<endl;
    system("pause");
    WSACleanup();
    return 0;
}
