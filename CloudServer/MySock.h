#pragma once
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<vector>

class MySock {
private:
	SOCKET handle;

public:
	static const int bufferSize = 1024;
	static void receiveLoop(void* socket) {
		MySock* sock = (MySock*)socket;
		while (true) {
			std::vector<char> msg = sock->receiveMsg();
			sock->sendMsg(msg);
		}
	}

	MySock(addrinfo*);
	MySock(SOCKET handle);

	MySock* acceptSock();
	std::vector<char> receiveMsg();
	void sendMsg(char* buffer, int length);
	void sendMsg(std::vector<char>);

	bool valid() { return handle != INVALID_SOCKET; }
};