// GameThread.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// 여러분들 보이시나요?

#include <iostream>
// 윈도우용 쓰레드 헤더
// #include <synchapi.h>
#include <process.h>
#include <functional>
#include <thread>
#include <Windows.h>

void FuncTest() {

}

// 쓰레드 예제코드를 치고
// 쓰레드의 특성에 대해서 알아볼겁니다.

// unsigned(__stdcall* _beginthreadex_proc_type)(void*);

static unsigned __stdcall ThreadFunc(void* arg) 
{
	// Busy Waiting

	// 계속 레지스터를 점유하고 있다.
	// 그 레지스터를 점유하는것을 그걸 컨텍스트 스위칭
	// 쓰레드를 잘못배우면 컨텍스트 스위칭이 
	// 프리락 

	while (true)
	{
		std::cout << "window Thread Process" << std::endl;
		Sleep(1);
	}

}

// 스택메모리가 존재하는데 스택메모리가 사라진겁니다.
void Func() 
{
	while (true)
	{
		// std::cout << "std Thread Process" << std::endl;
		Sleep(1);
	}
}

class A 
{
public:
	A(void (*Ptr)(int)) 
	{

	}

	A(void (*Ptr)(char))
	{

	}

	A(void (*Ptr)(char, int))
	{

	}

};

void Func() 
{

}

// _beginthreadex_proc_type;


void TestFunc(int a) 
{

}

// void(int) 함수포인터 타입
void (*Ptr)(int);

typedef void(*TestFunctionPtr)(int);

int main()
{
	// void (*Ptr)(int)

	// 우리가 어떤 메모리를 선언하면
	// 무조건 타입과
	// 위치가 생기게 됩니다.

	// A NewA = A(Func);
	// NewA => 이름이 곧 주소잖아요.
	// 0x182hj37d
	// 경험을 대입하는 

	// 윈도우의 쓰레드 함수입니다.
	// HANDLE ThreadHandle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, ThreadFunc, nullptr, 0, nullptr));

	// std:: 스탠다드 라이브러리
	
	// c++을 빌드가 되는 곳이라면 std를 사용해서 짠 코드는
	
	// 리눅스건 윈도우건 동일하게 돌아간다.

	// std::는 c++위원회가 만든다.

	// 리눅스 윈도우 상관없이 쓰레드를 만들수 있다.
    std::thread NewThread0 = std::thread(Func);
	std::thread NewThread1 = std::thread(TestFunc);
	std::thread NewThread2 = std::thread(std::bind(Func));

	// 쓰레드란 무엇이냐?
	// 동시에 2개의 함수가 돌아가는것.
	// 운영체제에게 요청해서 
	// main이 돌아가고 있지만
	// 내가 요청한 Func 함수도 같이 돌려줘.

	//while (true)
	//{
		std::cout << "main thread\n";
	// }

		// 쓰레드 핸들
	// WaitForSingleObject(ThreadHandle)

	NewThread0.join();

	// 우아한 종료를 해야한다.

	// 메인스택은 이미 끝나서 사라지게 되고
	// 기존에 돌려놨던 쓰레드는 계속 돌아가는데 
	// 쓰레드는 기본적으로 자신이 만들어졌든 프로그램에 귀속되고 영향을 받으므로
		// 메인이 종료되서 프로그램이 종료되었을때 남아있는 쓰레드가 있다면 에러가 나는건 당연한것.
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
