#include <iostream>
#include <mutex>
#include <Winsock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment (lib, "ws2_32")

std::thread RecvThread;
bool Check = true;

void RecvFunc(SOCKET _SessionSocket)
{
	std::string total;

	while (Check)
	{
		char Buffer[1024];

		int Result = recv(_SessionSocket, Buffer, sizeof(Buffer), 0);
		std::string temp = "";
		for (int i = 0; i < 1023; i++)
		{
			if (Buffer[i] == '\0')
			{
				total += temp;
				temp = "";
			}
			else
			{
				if (Buffer[i] == '!' && Buffer[i + 1] == '!')
				{
					Check = false;
					break;
				}
				temp += Buffer[i];
			}
		}

		if (SOCKET_ERROR == Result)
		{
			return;
		}
		std::cout << total << std::endl;
	}
	//std::cout << total << std::endl;
}

int main()
{
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}

	SOCKET SessionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == SessionSocket)
	{
		return 0;
	}

	std::string Ip;
	std::string Port;

	//Ip = "127.0.0.1";
	Ip = "58.150.30.210";

	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET;
	Add.sin_port = htons(30001);
	if (SOCKET_ERROR == inet_pton(AF_INET, Ip.c_str(), &Add.sin_addr))
	{
		return 0;
	}
	int Len = sizeof(SOCKADDR_IN);

	if (SOCKET_ERROR == connect(SessionSocket, (const sockaddr*)&Add, Len))
	{
		return 0;
	}

	std::cout << "커넥트 성공." << std::endl;

	RecvThread = std::thread(RecvFunc, SessionSocket);

	while (true)
	{
		std::string In;
		std::cin >> In;

		if (In == "q" || In == "Q")
		{
			closesocket(SessionSocket);
			Check = false;
			RecvThread.join();
			Sleep(1);
			return 0;
		}
		In = "최원준:" + In;
		char Buffer[1024] = { "" };

		Sleep(1);

		memcpy_s(Buffer, sizeof(Buffer), In.c_str(), In.size());
		int Result = send(SessionSocket, Buffer, sizeof(Buffer), 0);
	}
}
