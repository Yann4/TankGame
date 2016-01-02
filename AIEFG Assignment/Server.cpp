#include "Server.h"

Server::Server()
{
	
}

Server::~Server()
{
	if(run.load())
	{
		shutdownServer();
	}
}

void Server::turnOn()
{
	WSADATA wsaData;
	int retResult;
	if (retResult = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed: " << retResult;
		return;
	}

	//Create server socket
	listenSock = Socket::createSocket("10001");

	//Set up map of actions. The map uses a string as the key and an
	//std::function<Event(string, ClientIdentifer)> as the value
	/*actions["/Name"] = std::bind(&Server::changeName, this, std::placeholders::_1, std::placeholders::_2);
	actions["/Quit"] = std::bind(&Server::clientExit, this, std::placeholders::_1, std::placeholders::_2);
	actions["/Help"] = std::bind(&Server::sendHelpText, this, std::placeholders::_1, std::placeholders::_2);
	*/
	Actions::setAction("/Name", std::bind(&Server::changeName, this, std::placeholders::_1, std::placeholders::_2));
	Actions::setAction("/Quit", std::bind(&Server::clientExit, this, std::placeholders::_1, std::placeholders::_2));
	Actions::setAction("/Help", std::bind(&Server::sendHelpText, this, std::placeholders::_1, std::placeholders::_2));

	run.store(true);
	runThread = std::thread(&Server::update, this);
}

void Server::update()
{
	while(run.load())
	{
		if(!getNewClient(listenSock))
		{
			break;
		}
	}
	WSACleanup();
}

std::queue<std::string> Server::getMessages()
{
	if (allMessages.empty())
	{
		return std::queue<std::string>();
	}

	auto toRet = std::queue<std::string>();

	while (!allMessages.empty())
	{
		toRet.push(allMessages.front());
		allMessages.pop();
	}

	return toRet;
}

void Server::shutdownServer()
{
	run.store(false);
	for(int i = 0; i < clientThreads.size(); ++i)
	{
		clientThreads.at(i).join();
	}
	runThread.join();
}

//Messaging
void Server::broadcast(Message message)
{
	std::string mess = getClientIdentifier(message.senderID) + ": " + message.data + "\0";

	for(int i = 0; i < clients.size(); i++)
	{
			if(clients.at(i).id != message.senderID)
			{
				send(clients.at(i).sock, mess.c_str(), mess.length(), 0);
			}
	}
}

void Server::whisper(Message message)
{
	std::string mess = getClientIdentifier(-1) + ": " + message.data + "\0";

	for(int i = 0; i < clients.size(); i++)
	{
			if(clients.at(i).id == message.senderID)
			{
				send(clients.at(i).sock, mess.c_str(), mess.length(), 0);
				break;
			}
	}
}

//ClientIdentifer handlers
void Server::handleClient(ClientIdentifer client)
{
	std::string message = "";
	bool success = true;
	Actions::Event event = Actions::Event::ENTER;

	while(success && event != Actions::Event::QUIT && run.load())
	{
		message = "";
		success = Socket::listenTo(client.sock, message);

		if(success)
		{
			std::string mess = getClientIdentifier(client.id) + ": " + message;
			std::cout << mess << std::endl;
			allMessages.push(mess);
			event = handleMessage(message, client);
		}
	}

	//Shutdown client socket
	//Shutdown sending data (from client's perspective)
	if (shutdown(client.sock, SD_SEND) == SOCKET_ERROR)
	{
		std::cerr << "shutdown() failed: " << WSAGetLastError() << std::endl;
		closesocket(client.sock);
		WSACleanup();

		//Remove client from list of clients
		for(int i = 0; i < clients.size(); ++i)
		{
				if(clients.at(i).id == client.id)
				{
					clients.erase(clients.begin() + i);
					break;
				}
		}
		return;
	}

	//Shutdown recieving data (from client's perspective)
	closesocket(client.sock);

	//Remove client from list of clients
	for(int i = 0; i < clients.size(); ++i)
	{
			if(clients.at(i).id == client.id)
			{
				clients.erase(clients.begin() + i);
				break;
			}
	}
}

bool Server::getNewClient(SOCKET toListen)
{
	//Listen on a socket
	if (listen(toListen, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Listen failed with error: " << WSAGetLastError();
		closesocket(toListen);
		WSACleanup();
		return false;
	}

	//Accept a connection
	SOCKET clientSock = INVALID_SOCKET;

	fd_set readFD, writeFD, exceptFD;
	FD_ZERO(&readFD);
	FD_SET(toListen, &readFD);

	struct timeval tv = {0, 0};
	//If there is something waiting on the socket in the next [timeval] seconds, check that it's an incoming connection then accept it
	if(select(0, &readFD, NULL, NULL, &tv) > 0)
	{
		if(FD_ISSET(toListen, &readFD))
		{
			clientSock = accept(toListen, NULL, NULL);
			if (clientSock == INVALID_SOCKET)
			{
				std::cerr << "accept() failed: " << WSAGetLastError();
				closesocket(toListen);
				WSACleanup();
				return false;
			}

			ClientIdentifer newCli = ClientIdentifer("", ticketNumber++, clientSock);
			clients.push_back(newCli);
			clientThreads.push_back(std::thread(&Server::handleClient, this, newCli));
			whisper(Message(welcomeMessage, clients.back().id));
			whisper(Message("ID:" + std::to_string(clients.back().id), clients.back().id));
			return true;
		}
	}
	return true;
}

std::string Server::getClientIdentifier(int clientID)
{
	for(ClientIdentifer cli : clients)
	{
		if(cli.id == clientID)
		{
			if(!cli.name.empty())
			{
				return cli.name;
			}else
			{
				return std::to_string(clientID);
			}
		}
	}
	return "Server";
}

//Event handlers
Actions::Event Server::changeName(std::string message, ClientIdentifer client)
{
	for(int i = 0; i < clients.size(); i++)
	{
		if(clients.at(i).id == client.id)
		{
			//Position 5 should be the beginning of the next word after "/Name"
			clients.at(i).name = message.substr(6, std::string::npos);
			std::string shout = "[" + std::to_string(clients.at(i).id) + "] has changed their name to " + clients.at(i).name;
			broadcast(Message(shout, -1));
			return Actions::Event::NAME;
		}
	}
	return Actions::Event::SPEAK;
}

Actions::Event Server::clientExit(std::string message, ClientIdentifer client)
{
	broadcast(Message(getClientIdentifier(client.id) + " has left.", -1));
	return Actions::Event::QUIT;
}

Actions::Event Server::sendHelpText(std::string message, ClientIdentifer client)
{
	std::string helpText = "Welcome to the server. There are several commands that you can use.\n/Name [NewName here] changes your name\n/Quit to exit\n/Help to see this help text";
	whisper(Message(helpText, client.id));
	return Actions::Event::HELP;
}

Actions::Event Server::handleMessage(std::string message, ClientIdentifer client)
{
	auto func = Actions::getAction(message);
	if(func != nullptr)
	{
		return func(message, client);
	}

	broadcast(Message(message, client.id));
	return Actions::Event::SPEAK;
}
