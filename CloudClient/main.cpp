#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>

#define error(e) { printf("%s failed: %ld\n", e, WSAGetLastError()); }
#define DEFAULT_PORT	"2555"
#define DEFAULT_BUFF	1024
SOCKET serverSocket;

addrinfo* initWSA();
SOCKET connectTo(addrinfo*);

int main() {
	addrinfo* info = initWSA();
	SOCKET s = connectTo(info);

	while (true) {
		char inBuffer[1024];

		int outSize = scanf("%1023s", inBuffer);
		send(s, inBuffer, strlen(inBuffer), 0);

		int inSize = recv(s, inBuffer, 1024, 0);
		for (int i = 0; i < inSize; i++)
			printf("%i\n", inBuffer[i]);
		printf("\n");
	}


	shutdown(s, SD_SEND | SD_RECEIVE);
	closesocket(s);

	WSACleanup();
}

addrinfo* initWSA() {
	WSADATA wsaData;

	addrinfo *result, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = 0;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
		error("Startup");

	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
		error("Getaddrinfo");

	return result;
}

SOCKET connectTo(addrinfo* info) {
	addrinfo* ptr;
	SOCKET s;

	for (ptr = info; ptr != NULL; ptr = info->ai_next) {
		s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (s == INVALID_SOCKET) {
			error("socket");
			return s;
		}

		int res = connect(s, ptr->ai_addr, ptr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			closesocket(s);
			s = INVALID_SOCKET;
			continue;
		}

		return s;
	}
}