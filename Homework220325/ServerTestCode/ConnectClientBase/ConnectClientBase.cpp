// ConnectClientBase.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <conio.h>
#include <string>
#include <algorithm>
#include <vector>
#include <string>

#include <WinSock2.h>
#include <Windows.h> // 아래쪽에 놔야 합니다.
#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더

#pragma comment (lib, "ws2_32") // 서버통신용 구현이 들어있는 라이브러리

std::vector<std::string> CutIp(const std::string& _IpText, char _CutChar)
{
	// .12233.2222.333

	std::vector<std::string> Ips;
	Ips.push_back(std::string());
	for (size_t i = 0; i < _IpText.size(); i++)
	{
		if (_IpText[i] == _CutChar)
		{
			Ips.push_back(std::string());
			continue;
		}

		Ips.back() += _IpText[i];
	}

	return Ips;
}

bool IsNumberStr(const std::string& _Number)
{
	for (size_t i = 0; i < _Number.size(); i++)
	{
		if (0 == isdigit(_Number[i]))
		{
			return false;
		}
	}

	return true;
}

bool CheckIp(const std::string& _IpText) 
{
	std::vector<std::string> VectorIp = CutIp(_IpText, '.');

	if (4 != VectorIp.size())
	{
		return false;
	}

	// 숫자인지 아닌지 구분하면 끝난다.
	for (size_t i = 0; i < VectorIp.size(); i++)
	{
		if (VectorIp[i] == "")
		{
			return false;
		}
	}

	for (size_t i = 0; i < VectorIp.size(); i++)
	{
		if (false == IsNumberStr(VectorIp[i]))
		{
			return false;
		}

		int Value = std::stoi(VectorIp[i]);
		if (0 > Value)
		{
			return false;
		}

		if (Value > 256)
		{
			return false;
		}
	}

	int a = 0;

	// 내가 생각하는 완벽한 조건을 통과했을때만 여기로 오게 만든다.
	return true;
}

bool CheckPortNumber(const std::string& _IpText)
{
	return std::string::npos != _IpText.find(':');
}

int CheckPoint(const std::string& _IpText)
{
	int Count = 0;

	for (int i = 0; i < _IpText.size(); i++)
	{
		if (_IpText[i] == '.')
		{
			++Count;
		}
	}

	return Count;
}

int main()
{
	WSADATA wsa;

	// 윈도우에서 우리 프로그램이 소켓통신을 하겠다고 미리 알려주는 함수
	// MAKEWORD(2, 2) 소켓 통신 방식 버전을 알려준다.
	// 대부분 2.2 버전이기 때문에 아래와 같이 입력해주면 된다.
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}


	// 소켓통신을 위한 주소정보를 담는 구조체

	//         연결지속           종료
	// 프로토콜  SYN ACK
	// TCP 3 Way-Handshake & 4 Way-Handshake
	SOCKET SessionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 릴레이어블 UDP 
	// UDP 보냈어 상대인 서버가 받든 안받는 그건 상관 없는일.
	// SOCKET ServerConnectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == SessionSocket)
	{
		return 0;
	}

	// 클라는 바인드를 할필요가 없습니다.

	//std::cout << "접속 준비 키를 누르면 접속합니다" << std::endl;
	//_getch();


	//          0~255
	// 입력 예 : 192 . 211 . 333. 121
	// 입력 예 : 192 . 211 . 333
	// ip4가 아니면
	// 다시 입력
	// 입력 예 : 192.211.333.121:30000
	// 포트도 채워진다 ok?
	// 
	// 입력 예 : 192.211.255.121

	std::string ResultIp = "";
	std::string ResultPort = "";
	std::string Ip;
	int Port = -1;
	while (ResultIp == "")
	{
		std::cout << "IP주소를 입력해주세요" << std::endl;
		std::cin >> Ip;

		// 동시에 2가지를 처리하려고 하지마세요.
		if (3 != CheckPoint(Ip))
		{
			continue;
		}

		// 포트가 있는지 없는지 찾아봅니다.
		if (true == CheckPortNumber(Ip))
		{
			// 포트랑 ip를 분리해야한다.

			std::vector<std::string> IPAndPort = CutIp(Ip, ':');

			ResultIp = IPAndPort[0];
			ResultPort = IPAndPort[1];

		} else
		{
			if (true == CheckIp(Ip))
			{
				ResultIp = Ip;
				break;
			}
		}
	}

	while (ResultPort == "")
	{

	}

	int a = 0;





	// 여기서 두가지 상황을 두겠습니다.

	// 175.125.125.6:30000

	// 0.0.0.0

	// 포트가 입력되었을 때와
	// 입력되지 않았을때를 구분해라.
	// 입력받은후 꺼져야 하는 상황
	// IPv4가 아니면 종료

	// : 로 구분
	
	// 포트가 있을때 

	// + 포트를 입력했을때와 안했을때를 구분해서
	// 이사람이 포트를 입력하지 않았다면
	// 포트를 입력해주세요.
	// cin >> 

	// string에서 int가 되거나
	// int를 그대로 받을수 있게 만들었나?

	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET;
	Add.sin_port = htons(30001);
	if (SOCKET_ERROR == inet_pton(AF_INET, Ip.c_str(), &Add.sin_addr))
	{
		return 0;
	}
	int Len = sizeof(SOCKADDR_IN);

	// 세션(session)은 컴퓨터 과학에서, 
	// 특히 네트워크 분야에서 반영구적이고 상호작용적인 
	// 정보 교환을 전제하는 둘 이상의 통신 장치나 컴퓨터와 
	// 사용자 간의 대화나 송수신 연결상태를 의미하는 보안적인 
	// 다이얼로그(dialogue) 및 시간대를 가리킨다. 
	// 따라서 세션은 연결상태를 유지하는것보다 
	// 연결상태의 안정성을 더 중요시 하게된다.



	if (SOCKET_ERROR == connect(SessionSocket, (const sockaddr*)&Add, Len))
	{
		return 0;
	}

	char Buffer[1024] = {0};

	// 어떤 특정상황이 올때까지 기다려주는 함수를
	// 동기함수

	// 비동기는 

	
	// SYNC         ASYNC
	// 동기리시브와 비동기 
	// 기다리는것도 포함한다.
	int Result = recv(SessionSocket, Buffer, sizeof(Buffer), 0);

	if (SOCKET_ERROR == Result)
	{
		return 0;
	}


	// 이 소켓은 이제 서버와 연결된거에요.
	// ServerConnectSocket

	std::cout << "접속에 성공했습니다." << std::endl;

	_getch();

}
