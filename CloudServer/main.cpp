#include<WinSock2.h>
#include<stdio.h>
#include<WS2tcpip.h>
#include<process.h>
#include "MySock.h"

#define DEFAULT_PORT	"2555"
#define error(e) { printf("%s failed: %ld", e, WSAGetLastError()); }

SOCKET listenSocket;

addrinfo* initWSA();

int main() {
	addrinfo* addr = initWSA();
	MySock listening(addr);

	while (true) {
		printf("Listening to port %s ...\n", DEFAULT_PORT);

		MySock* newCli = listening.acceptSock();
		_beginthread(MySock::receiveLoop, 0, (void*)newCli);

		if (newCli->valid())
			printf("Client connected!\n");
		else
			break;	
	}

	system("pause");
}

addrinfo* initWSA() {
	int iResult;

	WSADATA data;

	iResult = WSAStartup(MAKEWORD(2, 2), &data);
	if (iResult != 0)
		error("Init");

	addrinfo hints, *result;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_socktype = SOCK_STREAM;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
		error("Getaddrinfo");

	return result;
}