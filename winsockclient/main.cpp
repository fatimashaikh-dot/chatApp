#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

/*
    initialize winsock

    create socket

    connect to the server

    send/recv

    close the socket
*/

bool initialize(){
    WSADATA data;
    return WSAStartup(MAKEWORD(2,2), &data) == 0;
}

int main(){

    if(!initialize()){
        cout<<"initialize winsock failed"<<endl;
    }

    //create socket
    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);//socket type
    if(s == INVALID_SOCKET){
        cout<<"Invalid socket created"<<endl;
        return 1;
    }

    int port= 12345;
    string serveraddress = "127.0.0.1";
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(serveraddress.c_str());

    if(connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR){//connecting ip and port
        cout<<"NOT able to connect the server"<<endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }

    cout<<"Successfully connected to server"<<endl;

    //send/recv
    string message = "Hello there!";
    int bytesent;
    bytesent = send(s, message.c_str(), message.length(), 0);

    if(bytesent == SOCKET_ERROR){
        cout<<"send failed"<<endl;
    }


    closesocket(s);

    WSACleanup();
    return 0;
}