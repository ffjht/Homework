// UDPClient.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
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

// udp 

// 컴퓨터 네트워킹에서, 레이어의 커뮤니케이션 
// 프로토콜의 최대 전송 단위(maximum transmission unit, MTU)란 
// 해당 레이어가 전송할 수 있는 최대 프로토콜 데이터 단위의 크기(바이트)이다.
// MTU 지표는 일반적으로 커뮤니케이션 인터페이스(네트워크 카드, 직렬 포트 등)와 
// 관련되어 나타난다.MTU 크기는 표준(이더넷)에 의해 결정될 수도 있고, 시스템이 
// 연결 시점에 결정될 수도 있다(점대점 시리얼 링크(point - to - point serial links)의 
// 경우).

// 보통 500바이트 정도를 잡습니다.
// 여기서 정말정말 주의해야할것이.
// udp Header(UDP 프토토콜)제외
// 450를 보내야 이래도 드랍됩니다.
// 확률을 높이려는 거지.
// 이러면 TCP 
// 온갖 인간의 꼼수.
// 3개씩 보내
// 5개씩 보낸다. 
// 놀랍게도. 패킷량이 더 커진다.
// 사람이 잘못짜죠?
// 릴레이어블 UDP 연결지향 UDP를 하겠다.

// 내가 잘 못짜.
// 아니 UDP왜쓰자고 했냐?
// TCP로 하자.

// TCP꼭 UDP보다 느려. <= 왜??? 와이? 
// 이유 다 통신 자체에 규약으로 어느정도 안전장치.

// 1. 네이글 알고리즘.(TCP만 적용됐다??) 끌수가 있다.
//    작은 패킷을 기관총처럼 쏜다 => 운영체제 적으로. 어 같은곳에 작은 패킷을 여러번쏘네?
//    내가 모아서 효율적으로 처리해줄께.

// 2. TCP 혼잡 방지 알고리즘
//    주변 패킷량이 많으면 기지국 수준에서 패킷을 정시킨다.
//    그 지역에 패킷량이 폭증하거나 하면.내께 느려져
//    이건 끌수 있을까요? 못꺼요.
//    프로그래머의 실력이 통신의 속력 자체를 늘릴수 있나?

// UDP 내가 쏘면 가.

// UDP는 쓸모가 없는건가요?

// 보통 이동패킷은 UDP 액션성이 정말 중요할때. 반응성 그안에서도 
//                                          1~2프레임 안갔다? 큰 영향을 안미쳐요.
//                                          60프레임 중에 2번 패킷이 누락됐다.
//                                          클라 => 서버 UDP
//                                          서버가 받으면 진짜 말도 안되는거 빼고 통과
//                                          오차를 감안합니다.
//                                          최대치 스피드핵

// 눈에 보이게 그럴듯하게 만드는겁니다. 유저들이 참을수 있는만큼 0.02부터 
// + 1초뒤 위치까지 보낸다거나. 

//                TCP                       서버 => 클라 TCP
//                                          확정 너는 여기야!
//											UDP로 짠 알고리즘이 
//                                          나는 쐈는데 안맞았으니 재가 렉이 있다느니.

int main()
{
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}

	SOCKET ClientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (INVALID_SOCKET == ClientSocket)
	{
		return 0;
	}

	SOCKADDR_IN SendAdd = { 0, };
	SendAdd.sin_family = AF_INET; // ip버전 4로 주소체계를 잡겠다.
	SendAdd.sin_port = htons(30001);

	// 0.0.0.0으로 하면 안된다. 로컬 호스트라는걸 모르게 된다.
	if (SOCKET_ERROR == inet_pton(AF_INET, "127.0.0.1", &SendAdd.sin_addr))
	{
		return 0;
	}

	while (true)
	{
		int SendAddSize = sizeof(SOCKADDR_IN);

		// int가 몇개?
		// 256 / 4
		char SendBuffer[256] = {0,};
		int RecvSize = 0;

		std::cout << "패킷을 입력해주세요" << std::endl;
		std::cin >> SendBuffer;

		RecvSize = sendto(ClientSocket, SendBuffer, sizeof(SendBuffer), 0,
			(struct sockaddr*)&SendAdd, sizeof(SOCKADDR_IN));

		if (RecvSize < 0)
		{
			closesocket(ClientSocket);
			WSACleanup();
			assert(false);
			return 0;
		}
	}

	closesocket(ClientSocket);
	WSACleanup();
}
