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
		if (temp[i] == ' ') continue;
		if (temp[i] < '0' || temp[i] > '9') return false;
		else cur = 10 * cur + int(temp[i] - '0');
	}

	if (cur > 255) return false;
	return true;
}

bool check_port(std::string temp)
{
	int cur = 0;

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] == ' ') continue;
		if (temp[i] < '0' || temp[i] > '9') return false;
		else
		{
			cur = 10 * cur + temp[i] - '0';
		}
	}

	if (cur > 65535) return false;
	return true;
}

int main()
{
	std::string Ip;
	int port = -1;
	while (true)
	{
		std::cout << "IP주소를 입력해주세요" << std::endl;
		getline(std::cin, Ip);

		if ("Q" == Ip
			|| "q" == Ip)
		{
			Ip = "127.0.0.1";
		}

		std::stringstream ss(Ip);
		std::string token;
		bool isIp = true, isPort = false;
		int ipCnt = 0;

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

						if (isPort) 
							port = stoi(temp);
					}
					lastIp = false;
				}
			}
		}

		if (!isIp) continue;
		while(!isPort)
		{
			std::string tempPort;
			std::cout << "포트를 입력해주세요\n";
			getline(std::cin, tempPort);
			isPort = check_port(tempPort);
			if (isPort) port = stoi(tempPort);
		}

		if (isIp && isPort) break;
	}
}