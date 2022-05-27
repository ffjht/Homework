// GameThreadOrder.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

// boost threadsafequeue라고 하는 멀티스레드에서 
// 쓰레드 핸들링을 하게해주는애

// iocp가 선생님은 성능이 좋은것 같은데.
// rio까지 나와서 

// window는 iocp
// Linux epoll

// 쓰레드 핸들링의 개념
// 쓰레드는 기본적으로 비지웨이팅(while(true))을 시키면 cpu를 많이 사용하는데
// 그런 쓰레드를 운영체제의 도움을 받아서 쉬게하고 있다가
// 정말 쓰레드의 연산력을 사용해야할때만 딱 깨우는 함수들과 기법들

// 코드적으로 이야기를 한다면(무슨 함수를 써야하지 무슨 클래스를 써야하지?)
// 쓰레드와 프로세스를 관리하는걸 os가 해주니까
// 운영체제와 밀접하므로 결국에는 운영체제의 함수와 기법을 지원해주는지
// 확인하고 그걸 써야하는데.

// window는 그냥 iocp쓰면 끝난다.

#include <iostream>
#include <thread>
#include <Windows.h>
#include <atomic>

// 쓰레드 핸들링
// 

std::atomic_bool Check = true;

void Func1() 
{
    while (Check)
    {
        Sleep(1000);
        int a = 0;
    }

    std::cout << "2번이 끝나면 시작합니다" << std::endl;

    // 쓰레드는 보통 중단점을 걸면 디버깅의 의미가 없어질때가 많습니다.
}

void Func2()
{
    std::cout << "2번 끝났다" << std::endl;
    Check = false;
}

int main()
{
    std::thread NewThread0 = std::thread(Func1);

    Sleep(100);

    std::thread NewThread1 = std::thread(Func2);

    NewThread0.join();
    NewThread1.join();
    
    
}
