// GameThread01.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <functional>

// 뮤텍스 세마포어 등등등등등........ 
// 동기화

// 메모리 접근 동기화에 사용하는 녀석입니다.
// 쓰레드가 동시에 같은 메모리에 접근하지 못하게 하는 
// 다른 쓰레가 이에 접근하려고 할때 대기(컨텍스트 스위칭)시키는 용도가 됩니다.

// 윈도우로 동기화를 배운 사람들은
// 크리티컬 섹션이라는 것은 기본적으로 개념을 의미한다.

// 임계 구역(critical section) 또는 공유변수 영역은 
// 병렬컴퓨팅에서 둘 이상의 스레드가 동시에 접근해서는 
// 안되는 공유 자원(자료 구조 또는 장치)을 접근하는 코드의 
// 일부를 말한다.임계 구역은 지정된 시간이 지난 후 종료된다.
// 때문에 어떤 스레드(태스크 또는 프로세스)가 임계 구역에 들어가고자 한다면 지정된 
// 시간만큼 대기해야 한다.스레드가 공유자원의 배타적인 사용을 보장받기 위해서 임계 구역에 들어가거나 나올때는 
// 세마포어 같은 동기화 매커니즘이 사용된다.


// 쓰레드당 하나를 만드는게 아닙니다.
// 공유하는 메모리 영역당 하나를 만드는게 기본입니다.
CRITICAL_SECTION MutexInst;

std::mutex STDMutex;

// 이녀석의 값을 참조하려고 할때는 문제가 
int PlayerHp = 3000000;

int ThreadResult0 = 0;
int ThreadResult1 = 0;


void MemFunc(int* _Ptr) 
{
    // STDMutex.lock();

    for (size_t i = 0; i < 100000; i++)
    {
        // 한줄이 한번에 처리되겠지?
        // 300000
        // 299990
        
        // 이걸 실행해주는 쓰레드가 이 0~3번을 한번에 다 실행할때까지
        // 다른 쓰레드는 이 PlayerHp에 접근 못하면 된다.
        
        // 0. PlayerHp의 주소값을 준비한다.
        // 1. PlayerHp의 주소값의 내부의 내용을 복사한다.
        // 2. 복사한 값을 -10 한다.
        // 3. 그 계산된 값을 다시 PlayerHp에 복사한다.

        // 운영체제에 대해서 어디까지 알아야 할까요?
        // 대부분의 상황에서 그 일이 쓸모없어 지거나 패치한번에 다 무용지물이 된다.
        // 쓰레드 0과 쓰레드 1이 
        
        // 먼저 들어오는 순간. 운영체제가 첫번째로 들어온 쓰레드를 제외하고는 
        // EnterCriticalSection(&MutexInst);


        (*_Ptr -= 10);

        // LeaveCriticalSection(&MutexInst);

        //EnterCriticalSection(&MutexInst);
        //(PlayerHp -= 10);
        //LeaveCriticalSection(&MutexInst);

    }

    // STDMutex.unlock();
}

int main()
{
    

    // const 가 안붙은 함수의 용법은? 들어갔다 나오면 
    // 니가 넣어준 녀석이 상태가 변경되어 있을것이다.
    // 운영체제가 해주겠죠.
    // 그럼 정리도 잘 해줘야 합니다.
    InitializeCriticalSection(&MutexInst);

    std::thread NewThread0 = std::thread(std::bind(MemFunc, &ThreadResult0));
    std::thread NewThread1 = std::thread(std::bind(MemFunc, &ThreadResult1));

    NewThread0.join();
    NewThread1.join();

    PlayerHp += ThreadResult0;
    PlayerHp += ThreadResult1;

    DeleteCriticalSection(&MutexInst);

    std::cout << PlayerHp << std::endl;

    // 충돌
    // 영역을 나누고
    // 그안에 있는 클래스들을 동시에 충돌

}