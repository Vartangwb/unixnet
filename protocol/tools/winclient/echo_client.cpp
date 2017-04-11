// echo_client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#pragma comment(lib,"WS2_32.lib")

int nonagle = 0;
int count = 1;

void setopt(int sockfd) {
	int ret, flag = 1;
	if (nonagle) {
		ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(flag));
		if (ret < 0) printf("set nodelay error, %d\n", WSAGetLastError());
		return;
	}
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		printf("Usage: %s <ip> <port> [SO_LINGER]\n", argv[0]);
		return 1;
	}
	if (argc >= 4) {
		count = atoi(argv[3]);
	}
	if (argc >= 5) {
		if (!strcmp("NONAGLE", argv[4])) {
			nonagle = 1;
		}
	}


	WSADATA wsd;
	SOCKET sockClient;  //�ͻ���socket
	SOCKADDR_IN addrSrv;
	int n, i;
	char recvBuf[64];

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		printf("start up failed!\n");
		return 1;
	}
	sockClient = socket(AF_INET, SOCK_STREAM, 0); //����socket
	setopt(sockClient);


	addrSrv.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(atoi(argv[2]));
	if (connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) != 0) { //���ӷ�������
		printf("connect error");
		return 1;
	}

	puts("connect success");

	while (1) {
		char c = getch();
		if (c == 'q') break;
		i = count;
		while(i--) send(sockClient, &c, 1, 0); //��������˷������ݣ��������� count �Ρ�
		n = recv(sockClient, recvBuf, 64, 0); //���շ�����������
		if (n < 0) {
			printf("read error, %d\n", WSAGetLastError());
			return 1;
		}
		recvBuf[n] = 0;

		printf("%s", recvBuf);
	}
	closesocket(sockClient); //�ر�����
	WSACleanup();
	return 0;
}


