#pragma once
#include <WinSock2.h>

struct ClientIdentifer
{
	std::string name;
	int id;
	SOCKET sock;

	ClientIdentifer()
	{
		name.clear();
		id = -1;
		sock = INVALID_SOCKET;
	}

	ClientIdentifer(std::string name, int id, SOCKET sock) : name(name), id(id), sock(sock)
	{	}
};
