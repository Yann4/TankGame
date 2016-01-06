#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <map>
#include <functional>
#include <queue>
#include <mutex>

#include "Socket.h"
#include "Structs.h"
#include "Actions.h"

struct Message
{
	std::string data;
	int senderID;

	Message():data(""), senderID(-1){}

	Message(std::string data, int senderID): data(data), senderID(senderID){}
};

class Server
{
private:
    std::vector<ClientIdentifer> clients;
    int ticketNumber = 0;

    std::vector<std::thread> clientThreads;
    std::thread runThread;
    std::atomic<bool> run;
    SOCKET listenSock;
	std::mutex messageMutex;

	std::string welcomeMessage;

	std::queue<std::string> allMessages;
public:
    Server();
    ~Server();
	void turnOn();

    void shutdownServer();
	
	void setWelcomeMessage(std::string message) {
		welcomeMessage = message;
	};

	void broadcast(Message message);
	void whisper(Message message);

	std::queue<std::string> getMessages();
private:
    std::string getClientIdentifier(int clientID);
    bool getNewClient(SOCKET toListen);
    void handleClient(ClientIdentifer client);

    void update();

    Actions::Event changeName(std::string message, ClientIdentifer client);
    Actions::Event clientExit(std::string message, ClientIdentifer client);
    Actions::Event sendHelpText(std::string message, ClientIdentifer client);
    Actions::Event handleMessage(std::string message, ClientIdentifer client);
};
