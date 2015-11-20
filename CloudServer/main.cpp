#include<stdio.h>
#include<algorithm>
#include<WinSock2.h>
#include<WS2tcpip.h>

#define error(e) { printf("%s failed: %ld\n", e, WSAGetLastError()); }
#define DEFAULT_PORT	"2555"
#define DEFAULT_BUFF	1024

addrinfo* initWSA();
SOCKET createListenSocket(addrinfo*);
SOCKET acceptClient(SOCKET);
void clientLoop(SOCKET);

int main() {
	addrinfo* info = initWSA();
	SOCKET listenSocket = createListenSocket(info);

	freeaddrinfo(info);

	while (true) {
		printf("Listening to port %s ...\n", DEFAULT_PORT);
		SOCKET client = acceptClient(listenSocket);
		clientLoop(client);
	}
}

addrinfo* initWSA() {
	WSADATA wsaData;

	addrinfo *result, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int iResult = 0;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
		error("Startup");

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
		error("Getaddrinfo");

	return result;
}

SOCKET createListenSocket(addrinfo* info) {
	int iResult;

	SOCKET s = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (s == INVALID_SOCKET)
		error("Socket");

	iResult = bind(s, info->ai_addr, (int)info->ai_addrlen);
	if (iResult != 0)
		error("Bind");

	iResult = listen(s, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
		error("Listen");

	return s;
}

SOCKET acceptClient(SOCKET listen) {
	SOCKET s = accept(listen, NULL, NULL);
	if (s == INVALID_SOCKET)
		error("Accept");

	return s;
}

void clientLoop(SOCKET client) {
	byte buffer[DEFAULT_BUFF];
	int bytesReceived;

	do {
		bytesReceived = recv(client, (char*)buffer, DEFAULT_BUFF, 0);

		if (bytesReceived > 0) {
			char* recBuffer = new char[bytesReceived + 1];
			CopyMemory(recBuffer, buffer, bytesReceived);
			recBuffer[bytesReceived] = '\0';

			printf("Received %i bytes...\n", bytesReceived);
			printf("%s\n", (char*)recBuffer);
			send(client, (char*)buffer, bytesReceived, 0);

			delete[] recBuffer;
		}
		else if (bytesReceived == 0)
			printf("Client diconnecting...\n\n");
		else
			error("Recv");
	} while (bytesReceived > 0);
}