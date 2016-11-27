#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

#pragma comment(lib, "ws2_32.lib") 

using namespace std;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 128

#define MAXSIZE_POOL 3

SOCKET cSock[MAXSIZE_POOL] = { INVALID_SOCKET };

class SocketServer
{
public:
	SocketServer(){}
	~SocketServer(){}
	void InitWinSock();
	void InitCheckListenSock();
	void BindSock();
	void ListenSock();
	void ShutDown();
	int existingClientCount = 0;
	SOCKET ListenSocket = INVALID_SOCKET;
private:
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
};


//Initialize WinSock
void SocketServer::InitWinSock()
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup Failed " << iResult << std::endl;
		WSACleanup();
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		cout << "Getaddrinfo failed with error " << iResult << endl;
		WSACleanup();
		//system("pause");
		return;
	}
}

//Initialize ListenSocket and double check
void SocketServer::InitCheckListenSock()
{
	ListenSocket = socket(result->ai_family,
		result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << "Error at socket() " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
}

//Bind socket to IP address or port
void SocketServer::BindSock()
{
	/*iResult = bind(ListenSocket, result->ai_addr,
		(int)result->ai_addrlen);*/
	bind(ListenSocket, result->ai_addr,
		(int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR)
	{
		std::cout << "Bind failed with error " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	/*Once bind function is called, the address information returned by the 
	*getadddrinfo function is no longer needed.The freeaddrinfo function is called to
	*free the memory allocated*/
	freeaddrinfo(result);
}

//Listen on a socket
void SocketServer::ListenSock()
{
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Listen failed with error: " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}


void SocketServer::ShutDown()
{
	for (auto& ClientSocket : cSock)
	{
		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			cout << "Shutdown failed with error" << WSAGetLastError() << endl;
			closesocket(ClientSocket);
			WSACleanup();
			//system("pause");
			return;
		}

		//Cleanup
		closesocket(ClientSocket);
		WSACleanup();
	}
	system("pause");
}


DWORD WINAPI ProcessClientRequests(int pid, LPVOID cParam, LPVOID sParam)
{
	SOCKET* clientSocket = (SOCKET*)cParam;
	SocketServer *socketServer = (SocketServer*)sParam;
	std::cout << "Thread " << pid << std::endl;
	char buff[DEFAULT_BUFLEN] = { NULL };
	std::string strSend;
	while (true) {
		int result = recv(*clientSocket, buff, DEFAULT_BUFLEN, 0);
		if (result > 0)
		{
			std::string strRecv = buff;
			if (strRecv == "$")
				break;
			std::cout << "Thread " << pid << " " << strRecv << std::endl;
			//Way to talk
			/*if (strRecv == "call0")
				send(cSock[0], "calling", 7, NULL);*/
			for (auto& b : buff) { b = NULL; }
		}
		else if (result == -1) //client window closed, release resources and return
		{
			std::cout << "Client " << pid << "'s window closed, connection terminated" << std::endl;
			closesocket(*clientSocket);

			socketServer->existingClientCount--;
			cSock[pid] = INVALID_SOCKET;

			return 0;
		}
	}	
	do {
		std::cout << "What to send to client " << pid << ": ";
		std::cin >> strSend;
		if (strSend != "$")
			send(*clientSocket, strSend.c_str(), strSend.length(), NULL);
	} while (strSend != "$");

	closesocket(*clientSocket);

	socketServer->existingClientCount--;
	cSock[pid] = INVALID_SOCKET;

	return 0;
}

#endif //!SOCKET_SERVER_H