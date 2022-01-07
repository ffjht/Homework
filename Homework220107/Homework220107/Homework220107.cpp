// ConnectClientBase.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>
#include <sstream>
#include <WinSock2.h>
#include <Windows.h> 
#include <WS2tcpip.h> 

#pragma comment (lib, "ws2_32")

bool check_Ip(std::string temp)
{
	int cur = 0;
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] < '0' || temp[i] > '9') return false;
		else cur = 10 * cur + temp[i] - '0';
	}

	if (cur > 255) return false;
}

bool check_port(std::string temp)
{
	int cur = 0;
	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] < '0' || temp[i] > '9') return false;
		else cur = 10 * cur + temp[i] - '0';
	}

	if (cur > 65535) return false;
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

	std::cout << "IP주소를 입력해주세요" << std::endl;
	std::string Ip;
	std::cin >> Ip;

	if ("Q" == Ip
		|| "q" == Ip)
	{
		Ip = "127.0.0.1";
	}

	std::stringstream ss(Ip);
	std::string token;
	bool isIp = true, isPort = false;
	int ipCnt = 0;
	u_short port;

	while (isIp && ipCnt < 4 && std::getline(ss, token, '.'))
	{
		ipCnt++;
		if (token.empty()) isIp = false;

		if (ipCnt < 4)
		{
			isIp = check_Ip(token);
		}
		else
		{
			std::stringstream ss(token);
			std::string temp;
			bool lastIp = true;
			while (std::getline(ss, temp, ':'))
			{
				if (lastIp)
				{
					isIp = check_Ip(temp);
				}
				else
				{
					isPort = check_port(temp);
					if (isPort) port = stoi(temp);
				}
				lastIp = false;
			}
		}
	}

	if (!isIp) return 0;
	if (!isPort)
	{
		std::cout << "포트를 입력해주세요\n";
		std::cin >> port;
	}

	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET;
	Add.sin_port = htons(port);							//
	if (SOCKET_ERROR == inet_pton(AF_INET, Ip.c_str(), &Add.sin_addr))
	{
		return 0;
	}
	int Len = sizeof(SOCKADDR_IN);

	if (SOCKET_ERROR == connect(SessionSocket, (const sockaddr*)&Add, Len))
	{
		return 0;
	}

	char Buffer[1024] = { 0 };

	int Result = recv(SessionSocket, Buffer, sizeof(Buffer), 0);

	if (SOCKET_ERROR == Result)
	{
		return 0;
	}

	std::cout << "접속에 성공했습니다." << std::endl;

	_getch();

}