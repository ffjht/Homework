// IOCPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <conio.h>
#include <functional>
#include <assert.h>

#include <WinSock2.h>
#include <Windows.h> // 아래쪽에 놔야 합니다.
#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더
#include <thread>
#include <memory>
#include <vector>
#include <mutex>

#pragma comment (lib, "ws2_32") // 서버통신용 구현이 들어있는 라이브러리


		//SOCKET s = SessionSocket,
		//LPWSABUF lpBuffers,
		//DWORD dwBufferCount,
		//LPDWORD lpNumberOfBytesRecvd,
		//LPDWORD lpFlags,
		//LPWSAOVERLAPPED lpOverlapped,
		//LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine

// OVERLAPPED 역할은?
// 운영체제가 이녀석을 가져가고
// 

// 비동기 io (입 출력)
// Iocp는 소켓 통신에서만 쓸수 있나?
// 소켓통신이라는건 대체 뭐지?
// 근본적으로 본다면
// 5기가짜리 파일입니다.
// 윈도우는 우리에게 우리는 이걸 기반으로 해서
// 비동기 데이터 입출력 그때 지금까지 한 결과와 
// 이런것들에 대한 정보를 OVERLAPPED라는 구조체를 통해서 관리하고
// 우리에게 전달해주기 때문에.
// 우리가 한번 예약한 recv에 대한 정보는 하나의 OVERLAPPED

class RECVOVERDATA 
{
public:
	SOCKET Socket;
	WSABUF Buf;
	DWORD dwBufferCount;
	char ArrBuffer[1024];
	DWORD NumberOfBytesRecvd;
	DWORD Flags;
	OVERLAPPED Overlapped;

public:
	RECVOVERDATA() 
	{
		memset(this, 0, sizeof(RECVOVERDATA));

		Buf.len = 1024;
		Buf.buf = ArrBuffer;
	}
};

HANDLE Iocp_;
std::mutex iocpLock;
std::vector<std::shared_ptr<std::thread>> threadList_;


void IocpThread() 
{
	while (true)
	{
		// 일이 10개여서 1번씩 10번깨어나는것과
		// 일이 10개인데 /n
		DWORD NumberOfBytesTransferred;
		ULONG_PTR CompletionKey;
		LPOVERLAPPED lpOverlapped;
		int Value =  GetQueuedCompletionStatus(Iocp_, &NumberOfBytesTransferred, &CompletionKey, &lpOverlapped, INFINITE);




		// GetQueuedCompletionStatusEx

		RECVOVERDATA* RecvData =  reinterpret_cast<RECVOVERDATA*>(CompletionKey);


		std::cout << RecvData->ArrBuffer << std::endl;

		if (SOCKET_ERROR == WSARecv(
			RecvData->Socket,
			&RecvData->Buf,
			1,
			&RecvData->NumberOfBytesRecvd,
			&RecvData->Flags,
			&RecvData->Overlapped,
			nullptr))
		{
			int Error = WSAGetLastError();

			// 997L
			// WSA_IO_PENDING 비동기 입출력 준비를 하고 있다는 소리이므로
			// 에러로 처리하면 안된다.
			if (WSA_IO_PENDING != Error)
			{
				assert(false);
			}
		}

	}

}



int main()
{
	WSADATA wsa;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		std::cout << "WSAStartup Error" << std::endl;
	}


	Iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 8);

	for (size_t i = 0; i < 8; i++)
	{
		threadList_.push_back(std::make_shared<std::thread>(IocpThread));
	}

	SOCKET ServerAccpetSocket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == ServerAccpetSocket)
	{
		return 0;
	}

	{
		// 소켓옵션 바꿔주는 함수
		// 기본적으로 여러분들이 아무것도 없이 소켓을 만들면
		// 당연히 소켓의 설정은 기본값으로 되어있을것이다. 그중 몇가지 옵션을
		// 바꿔줄수 있는 함수이다.
		/* set/clear non-blocking i/o */
		unsigned long Mode = 1;
		int Result = ioctlsocket(ServerAccpetSocket, FIONBIO, &Mode);

		// SOCKET_ERROR 도대체 무슨 소켓에러가 났는데?
		if (SOCKET_ERROR == Result)
		{
			DWORD Code = GetLastError();
			assert(false);
			return 0;
		}
	}


	SOCKADDR_IN Add = { 0, };
	Add.sin_family = AF_INET; // ip버전 4로 주소체계를 잡겠다.
	Add.sin_port = htons(30001);

	if (SOCKET_ERROR == inet_pton(AF_INET, "0.0.0.0", &Add.sin_addr))
	{
		return 0;
	}

	std::cout << "server start" << std::endl;

	// socket비동기 기본형 소켓을 만들썼다.
	// Windows Socket API
	// 순서가 다 명확하게 기억
	// SOCKET ServerAccpetSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// 이녀석을 통해서 접속한 소켓도 비동기가 가능해진다.


	if (SOCKET_ERROR == bind(ServerAccpetSocket, (const sockaddr*)&Add, sizeof(SOCKADDR_IN)))
	{
		return 0;
	}

	if (SOCKET_ERROR == listen(ServerAccpetSocket, 512))
	{
		return 0;
	}

	std::cout << "서버 스타트" << std::endl;

	while (true)
	{
		SOCKADDR_IN UserAdd = { 0, };
		UserAdd.sin_family = AF_INET;
		int Len = sizeof(SOCKADDR_IN);

		// 정리
		//  ioctlsocket(ServerAccpetSocket, FIONBIO, &Mode);
		// 이 옵션을 켜고 동기 함수를 사용하면
		// 동기 함수들은 리턴하면서 에러라고 뱉기 시작한다.
		// 이게 진짜 에러인지를 파악하려면
		// WSAGetLastError을 통해서 에러의 상황을 판단해서 처리해줘야 한다.
		SOCKET SessionSocket = accept(ServerAccpetSocket, (sockaddr*)&UserAdd, &Len);

		if (INVALID_SOCKET == SessionSocket)
		{
			int Error = WSAGetLastError();

			if (WSAEWOULDBLOCK == Error)
			{
				Sleep(1);
				continue;
			}
			else 
			{
				assert(false);
				// 진짜 뭔가 잘못됐다.
			}
		}

		std::cout << "접속자 들어옴 키를 눌러주세요." << std::endl;

		RECVOVERDATA* NewData = new RECVOVERDATA();


		NewData->Socket = SessionSocket;

		// 2가지 
		CreateIoCompletionPort((HANDLE)SessionSocket, Iocp_, (ULONG_PTR)NewData, 0);

		WSABUF _buf;

		//SOCKET s = SessionSocket,
		//LPWSABUF lpBuffers,
		//DWORD dwBufferCount,
		//LPDWORD lpNumberOfBytesRecvd,
		//LPDWORD lpFlags,
		//LPWSAOVERLAPPED lpOverlapped,
		//LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine


		// NewData->Buf을 10개 넣어줬다면 

		// IOCP야 나 리시브 등록해줘 뭔가 오면 니가 체크해줘
		if (SOCKET_ERROR == WSARecv(
			NewData->Socket, 
			&NewData->Buf,
			1,
			&NewData->NumberOfBytesRecvd, 
			&NewData->Flags,
			&NewData->Overlapped,
			nullptr))
		{
			int Error = WSAGetLastError();

			// 997L
			// WSA_IO_PENDING 비동기 입출력 준비를 하고 있다는 소리이므로
			// 에러로 처리하면 안된다.
			if (WSA_IO_PENDING != Error)
			{
				assert(false);
			}
		}


		int a = 0;
	}

	// closesocket(SessionSocket);
}
