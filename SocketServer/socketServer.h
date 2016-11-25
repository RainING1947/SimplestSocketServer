#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <stdlib.h>
#include <string>

#pragma comment(lib, "ws2_32.lib") 

using namespace std;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 128

class SocketServer
{
public:
	SocketServer(){}
	~SocketServer(){}
	void PrepareSock();
	void InitWinSock();
	void InitCheckListenSock();
	void BindSock();
	void ListenSock();
	void AcceptSock();
	void RecvAndSend();
	void Receive();
	void ClearRecvBuf();
	void Send(std::string sendStr);
	void ShutDown();
	bool GetSessionState(); //iResult>0 loop send and recv, iResult=0 closing connection, iResult<0 error
	std::string GetRecvStr();
private:
	WSADATA wsaData;
	int iResult;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	char recvbuf[DEFAULT_BUFLEN] = { ' ' };
	int iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;
};

void SocketServer::PrepareSock()
{
	this->InitWinSock();
	this->InitCheckListenSock();
	this->BindSock();
	this->ListenSock();
	this->AcceptSock();
}

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
	iResult = bind(ListenSocket, result->ai_addr,
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

void SocketServer::AcceptSock()
{
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		std::cout << "Accept failed with error " << WSAGetLastError() << std::endl;
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
}

void SocketServer::RecvAndSend()
{
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			cout << "Bytes received: " << iResult << " " << recvbuf << endl;

			string recvStr = recvbuf;
			cout << "String got: " + recvStr << endl;

			//Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				cout << "Send failed " << WSAGetLastError() << endl;
				closesocket(ClientSocket);
				WSACleanup();
				//system("pause");
				return;
			}

			cout << "Bytes sent: " << iSendResult << " " << recvbuf << endl;
		}
		else if (iResult == 0)
		{
			cout << "Connection Closing..." << endl;
		}
		else
		{
			cout << "recv failed with error " << WSAGetLastError();
			closesocket(ClientSocket);
			WSACleanup();
			//system("pause");
			return;
		}

	} while (iResult > 0);
}

void SocketServer::Receive()
{
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (iResult > 0)
	{
		cout << "Bytes received: " << iResult << " " << recvbuf << endl;
	}
	else if (iResult == 0)
	{
		cout << "Connection Closing..." << endl;
	}
	else
	{
		cout << "recv failed with error " << WSAGetLastError();
		closesocket(ClientSocket);
		WSACleanup();
		//system("pause");
		return;
	}
}

void SocketServer::ClearRecvBuf()
{
	memset(recvbuf, ' ', sizeof(recvbuf));
}

void SocketServer::Send(std::string sendStr)
{
	//Echo the buffer back to the sender
	iSendResult = send(ClientSocket, sendStr.c_str(), sendStr.length(), 0);
	if (iSendResult == SOCKET_ERROR)
	{
		cout << "Send failed " << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		//system("pause");
		return;
	}
	cout << "String sent: " << iSendResult << " " << sendStr << endl;
}

void SocketServer::ShutDown()
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

	system("pause");
}

bool SocketServer::GetSessionState()
{
	return (bool)iResult;
}

std::string SocketServer::GetRecvStr()
{
	std::string recvStr = recvbuf;
	return recvStr;
}

#endif //!SOCKET_SERVER_H