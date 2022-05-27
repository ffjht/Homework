// AccpetServerBase.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>

#include <WinSock2.h>
#include <Windows.h> // 아래쪽에 놔야 합니다.
#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더
#include <thread>
#include <memory>
#include <vector>
#include <mutex>

#pragma comment (lib, "ws2_32") // 서버통신용 구현이 들어있는 라이브러리

std::mutex SessionLock;
std::vector<std::shared_ptr<std::thread>> ThreadList;
std::vector<SOCKET> SessionSockets;

void UserThreadFunc(SOCKET SessionSocket)
{
	while (true)
	{
		char Buffer[1024];

		int Result = recv(SessionSocket, Buffer, sizeof(Buffer), 0);

		if (-1 == Result)
		{
			DWORD error = WSAGetLastError();
			char* message = nullptr;

			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				nullptr,
				error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(char*)&message,
				0,
				nullptr);

			if (nullptr != message)
			{
				printf_s("Code : %d Message : %s", error, message);
				// 운영체제가 준건 해제헤야 한다.
				// 서버면 더더더더더더욱
				// 실행중 발생하는 릭이 가장 위험하다.
				// 힙이 비대해지기 시작합니다.
				// 램을 초과하면서 터진다.
				LocalFree(message);
			}
			return;
		}

		std::cout << Buffer << std::endl;
		SessionLock.lock();

		for (size_t i = 0; i < SessionSockets.size(); i++)
		{
			if (SessionSockets[i] == SessionSocket)
			{
				continue;
			}

			send(SessionSockets[i], Buffer, sizeof(Buffer), 0);
		}

		SessionLock.unlock();

	}
}

int main()
{

	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}

	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET; // ip버전 4로 주소체계를 잡겠다.
	Add.sin_port = htons(30001);

	if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
	{
		return 0;
	}

	std::cout << "server start" << std::endl;

	SOCKET ServerAccpetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == ServerAccpetSocket)
	{
		return 0;
	}

	if (SOCKET_ERROR == bind(ServerAccpetSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
	{
		return 0;
	}

	if (SOCKET_ERROR == listen(ServerAccpetSocket, 512))
	{
		return 0;
	}

	while (true)
	{
		SOCKADDR_IN UserAdd = { 0, };
		int Len = sizeof(SOCKADDR_IN);
		std::cout << "서버 접속 함수 실행" << std::endl;

		// 동기 액셉트
		SOCKET SessionSocket = accept(ServerAccpetSocket, (sockaddr*)&UserAdd, &Len);
		std::cout << "접속자 들어옴 키를 눌러주세요." << std::endl;

		if (INVALID_SOCKET == SessionSocket)
		{
			return 0;
		}

		SessionLock.lock();
		ThreadList.push_back(std::make_shared<std::thread>(UserThreadFunc, SessionSocket));
		SessionSockets.push_back(SessionSocket);
		SessionLock.unlock();
	}
	_getch();
}