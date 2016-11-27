#include "socketServer.h"
#include <thread>
#include <memory.h>

const std::string Permision = "permision";
const std::string Deny = "Thread pool is full, please wait";

std::thread threads[MAXSIZE_POOL];

int main()
{
	SocketServer *socketServer = new SocketServer();
	memset(cSock, INVALID_SOCKET, sizeof(cSock));

	socketServer->InitWinSock();
	socketServer->InitCheckListenSock();
	socketServer->BindSock();
	socketServer->ListenSock();

	while (true)
	{
		if (socketServer->existingClientCount < MAXSIZE_POOL)
			std::cout << "Waiting Client to connect" << std::endl;
		SOCKET tmpSock = accept(socketServer->ListenSocket, NULL, NULL);
		if (socketServer->existingClientCount < MAXSIZE_POOL)
		{
			send(tmpSock, Permision.c_str(), Permision.length(), NULL);
			for (int i = 0; i < MAXSIZE_POOL; i++)
			{
				if (cSock[i] == INVALID_SOCKET)
				{
					cSock[i] = std::move(tmpSock);
					threads[i] =
						std::thread(ProcessClientRequests, i, &cSock[i], socketServer);
					threads[i].detach(); //A thread returns and release resources BY ITSELF
					socketServer->existingClientCount++;
					break;
				}
			}
		}
		else
		{
			send(tmpSock, Deny.c_str(), Deny.length(), NULL);
		}
	}
	
	socketServer->ShutDown();
	delete socketServer;

	std::cin.get();
	return 0;
}
