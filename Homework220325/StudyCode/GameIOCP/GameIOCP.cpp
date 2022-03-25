// GameIOCP.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>
#include <thread>
#include <functional>
#include <vector>
#include <assert.h>
#include <conio.h>

// 윈도우에서는 대부분 핸들방식을 사용합니다.
// 윈도우는 뭔가 중요한 윈도우운영체제에서 지원해주는
// 객체(윈도우창)나 혹은 윈도우 기능들을 우리가 사용하겠다고하면
// 윈도우는 우리에게 window* 주지 않아. 
// 저렇게 주면 우리에게 메모리 위치 그자체를 주는거죠?
// HANDLE 
// 92
// 8바이트 정수

// 비유로 보자면 윈도우가 map<프로그램, list<int64>> 여러가지 기능들을
// 제공해주는것. 

// 윈도우 기능을 이용하는 단계는 보통 다음과 같다.
// 1. 윈도우에게 기능을 사용하겠다고 알린다.
// 2. 윈도우가 우리에게 HANDLE을 하나 준다. 모든 핸들은 사실 결국 8바이트 정수야
// 3. 그 기능을 이용는 함수들을 할때마다 HANDLE을 달라고 한다.
// 4. 윈도우가 핸들을 받으면 알아서 이게 유요한지 안한지 처리해서 기능을 써준다.

HANDLE gIocp_ = 0;

// IOCP는 쓰레드 핸들링을 위한 윈도우 지원 인터페이스
// (제공하는 방법)

void ThreadFunction(int ThreadId)
{
	// 쓰레드를 쓸때 가장 큰 문제점
	// 1. 메모리 동시접근
	// 2. 쓸데없이 쓰레드가 일하는 문제(필요할때만 깨우는 문제.)

	// 부호없는 4바이트 정수
	DWORD NumberOfBytesTransferred;
	ULONG_PTR CompletionKey;
	LPOVERLAPPED lpOverlapped = nullptr;

	// 서버통신을 하게될때 필요한 녀석입니다.
	// OVERLAPPED * Overlapped

	//GetQueuedCompletionStatus(
	//	_In_ HANDLE CompletionPort,
	//	_Out_ LPDWORD lpNumberOfBytesTransferred,
	//	_Out_ PULONG_PTR lpCompletionKey,
	//	_Out_ LPOVERLAPPED * lpOverlapped,
	//	_In_ DWORD dwMilliseconds
	//);

	while (true)
	{
		// 할일도 없는데 cpu는 그냥 와창창 쓰고 있다.
		// 일이 생길때까지 대기함수.
		GetQueuedCompletionStatus(gIocp_, &NumberOfBytesTransferred, &CompletionKey, &lpOverlapped, INFINITE);

		if (NumberOfBytesTransferred == -1)
		{
			break;
		}

		// 일이라는 건 뭐고.
		// 함수 포인터 형식으로 날아올테니
		// 함수를 실행하면

		std::cout << ThreadId << " Thread Process" << std::endl;
		std::cout << NumberOfBytesTransferred << " NumberOfBytesTransferred" << std::endl;

		void(*Ptr)() = reinterpret_cast<void(*)()>(CompletionKey);
		Ptr();
		std::cout << CompletionKey << " CompletionKey" << std::endl;
	}
}

void PlayerAtt() 
{
	std::cout << "플레이어가 공격합니다" << std::endl;
}

int main()
{
	// 윈도우 지원 함수.
	SYSTEM_INFO Sys;
	GetSystemInfo(&Sys);

	int ThreadCount = Sys.dwNumberOfProcessors;

	// CreateIoCompletionPortb 함수는 두가지 기능으로 이용되는데.
	// 첫번째는 iocp를 사용하겠다고 할때
	// 핸들을 리턴해주는 용도로 사용합니다.
	// 만들때의 사용법이 좀 다 기본값으로 해서 핸들을 만드는 용도로 사용할때.

	// 두번째 경우는 서버통신을 공부할때 배우게 될겁니다.
	gIocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, ThreadCount);

	if (INVALID_HANDLE_VALUE == gIocp_)
	{
		assert(false);
		return 0;
	}

	std::vector<std::thread*> ThreadVector;

	for (size_t i = 0; i < ThreadCount; i++)
	{
		ThreadVector.push_back(new std::thread(std::bind(ThreadFunction, i)));
	}

	while (true)
	{
		int Value = _getch();

		if (Value == 'q')
		{
			break;
		}
		else if (0 != Value)
		{
			PostQueuedCompletionStatus(gIocp_, 50, reinterpret_cast<ULONG_PTR>(&PlayerAtt), nullptr);
		}
		
	}


	for (size_t i = 0; i < ThreadVector.size(); i++)
	{
		PostQueuedCompletionStatus(gIocp_, -1, reinterpret_cast<ULONG_PTR>(&PlayerAtt), nullptr);
	}

	for (size_t i = 0; i < ThreadVector.size(); i++)
	{
		ThreadVector[i]->join();
	}

	for (size_t i = 0; i < ThreadVector.size(); i++)
	{
		delete ThreadVector[i];
	}
}
