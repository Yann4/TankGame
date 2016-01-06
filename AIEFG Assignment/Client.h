#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <queue>
#include <iostream>
#include <mutex>

#include "Socket.h"

class Client
{
private:
    std::atomic<bool> socketOpen = false;
    SOCKET sock;

    std::thread getMessageThread;
	
	std::mutex sendMessageMutex;
	std::mutex recvMessageMutex;
    std::queue<std::string> toSend;
    std::queue<std::string> recieved;
public:
    Client();
    Client(std::string serverName, std::string port);
	void Initialise(std::string serverName, std::string port);

    ~Client();
    void Shutdown();

    void pushMessage(std::string);
    void update();

    std::queue<std::string> getMessages();
private:
    void recieveMessage(SOCKET listenOn);
    void connectToServer(std::string serverName, std::string port);
};
