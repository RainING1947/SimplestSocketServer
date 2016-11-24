#include "socketServer.h"

int main()
{
	SocketServer *socketServer = new SocketServer();
	socketServer->PrepareSock();
	do {
		socketServer->Receive();
		if (socketServer->GetSessionState())
		{
			std::cout << socketServer->GetRecvStr() << std::endl;
			socketServer->Send("HelloWord");
		}
	} while (socketServer->GetSessionState());

	socketServer->ShutDown();
	return 0;
}