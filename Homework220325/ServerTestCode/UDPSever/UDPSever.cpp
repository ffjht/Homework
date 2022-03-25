// UDPSever.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>
#include <assert.h>

#include <WinSock2.h>
#include <Windows.h> // 아래쪽에 놔야 합니다.
#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더
#include <thread>
#include <memory>
#include <vector>
#include <mutex>

#pragma comment (lib, "ws2_32") // 서버통신용 구현이 들어있는 라이브러리

int main()
{
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}

	SOCKET ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == ServerSocket)
	{
		return 0;
	}


	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET; // ip버전 4로 주소체계를 잡겠다.
	Add.sin_port = htons(30001);

	if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
	{
		return 0;
	}

	std::cout << "server start" << std::endl;



	if (SOCKET_ERROR == bind(ServerSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
	{
		return 0;
	}

	//if (SOCKET_ERROR == listen(ServerAccpetSocket, 512))
	//{
	//	return 0;
	//}


	while (true)
	{
		SOCKADDR_IN RecvAdd = { 0, };
		int RecvAddSize = sizeof(SOCKADDR_IN);
		char RecvBuffer[256];
		int RecvSize = 0;

		RecvSize = recvfrom(ServerSocket, RecvBuffer, sizeof(RecvBuffer), 0, (struct sockaddr*)&RecvAdd, &RecvAddSize);

		if (RecvSize < 0)
		{
			closesocket(ServerSocket);
			WSACleanup();
			assert(false);
			return 0;
		}

		std::cout << "패킷의 데이터는 : " << RecvBuffer << std::endl;

	}

	closesocket(ServerSocket);
	WSACleanup();

	_getch();

}
