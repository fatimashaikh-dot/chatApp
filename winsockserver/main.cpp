//server 
#include<iostream>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<tchar.h>
#include<thread>
#include<algorithm>
#include<vector>
#include<functional>

using namespace std;

#pragma comment(lib,"ws2_32.lib")

/*
    //intialize winsock library

    //create the socket

    //get ip and port

    //bind the ip/port with the socket

    //accept

    //rec the socket

    //close socket

    //cleanup the winsock
*/
bool initialize(){
    WSADATA data;
    return WSAStartup(MAKEWORD(2,2), &data) == 0;
}

void IneractWithClient(SOCKET clientSocket, vector<SOCKET>&clients){
    //send/recv client

    cout<<"client connected"<<endl;

    char buffer[1046];

    while (1){
     int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);//receive API

     if(bytesrecvd <=0){
        cout<<"Client disconnected"<<endl;
        break;
     }

    string message(buffer, bytesrecvd);
    cout<<"message from client: "<<message<<endl;

    for(auto client : clients){
        if(client != clientSocket){//only send message to new clients
        send(client, message.c_str(), message.length(), 0);
        }
    }

    }

    auto it = find(clients.begin(), clients.end(), clientSocket);//to remove closed socket from vector
    if(it != clients.end()){
        clients.erase(it);
    }


    closesocket(clientSocket);
}

int main(){
   if (!initialize()){
        cout<<"winsock initialization failed"<<endl;
        return 1;
   }
    cout<<"server program started"<<endl;

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(listenSocket == INVALID_SOCKET){
        cout<<"Socket creation failed"<<endl;
        return 1;
    }

    //create address structure
    int port=12345;
    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

    //convert the ip address (0.0.0.0) put it inside the sin_family in binary format
    serveraddr.sin_addr.s_addr = INADDR_ANY;//API if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1){ 

    //bind
    if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR){
        cout<<"bind failed"<<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    //listen
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){
        cout<<"listen failed"<<endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    cout<<"Server has started listening on port: "<<port<<endl;
    vector<SOCKET> clients;

    //accept
    while(1){
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
    if(clientSocket == INVALID_SOCKET){
        cout<<"Accept failed"<<endl;
    }

    clients.push_back(clientSocket);
    thread t1(IneractWithClient, clientSocket, std::ref(clients));
    t1.detach();
    }

    closesocket(listenSocket);

    WSACleanup();  //finalize
    return 0;
}