#include "MySock.h"
#include<stdio.h>

MySock::MySock(SOCKET handle) {
	this->handle = handle;
}

MySock::MySock(addrinfo* addr) { //LISTEN SOCK
	handle = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (handle == INVALID_SOCKET) {
		printf("Couldn't initialize listening socket...\nError code: %ld", WSAGetLastError());
		return;
	}

	int iResult;

	iResult = bind(handle, addr->ai_addr, addr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("Couldn't bind socket: %li", WSAGetLastError());
		return;
	}

	iResult = listen(handle, SOMAXCONN);
	if (iResult) {
		printf("Coudln't listen: %li", WSAGetLastError());
		return;
	}
}

MySock* MySock::acceptSock() {
	SOCKET sock = accept(handle, nullptr, nullptr);
	if (sock == INVALID_SOCKET) {
		printf("Couldn't accept socket...\nError code: %ld", WSAGetLastError());
		return new MySock(INVALID_SOCKET);
	}

	return new MySock(sock);
}

std::vector<char> MySock::receiveMsg() {
	char buffer[bufferSize];
	int bytesReceived = recv(handle, buffer, bufferSize, 0);

	return std::vector<char>(buffer, buffer + bytesReceived);
}

void MySock::sendMsg(char* data, int length) {
	sendMsg(std::vector<char>(data, data + length));
}
void MySock::sendMsg(std::vector<char> data) {
	send(handle, &data[0], data.size(), 0);
}
