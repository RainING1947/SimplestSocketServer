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
			socketServer->ClearRecvBuf();
		}
	} while (socketServer->GetSessionState());

	std::string str;	
	do {
		std::cout << "What to send to client: ";
		std::cin >> str;
		if (str != "$")
			socketServer->Send(str);
	} while (str != "$");

	socketServer->ShutDown(); //this Shutdown cleanup resources which means connection terminate(differs from that of client)
	
	delete socketServer;

	std::cin.get();
	return 0;
}