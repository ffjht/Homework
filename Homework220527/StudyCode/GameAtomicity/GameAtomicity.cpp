// GameAtomicity.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <list>
#include <mutex>


// 64비트 자료형까지는 지켜줬던것으로 기억한다.
// using atomic_int    = atomic<int>;
// 하드웨어 수준의 원자성을 보장하게 된다.

// 64비트 크기 이하자료형때문에 굳이 락을 걸어야 할일이 생긴다면
// 이 atomic을 사용하는 것을 생각해보세요.
std::atomic_int TestNumber = 0;
// int TestNumber = 0;

class Player 
{
public:
    std::mutex LockHp;
    int Hp;

    std::mutex LockAtt;
    int Att;

public:
    void Test() 
    {
        this->Att;
    }
};

// 메모리영역으로 치면? 데이터영역입니다.
std::list<Player*> PlayerList;


class List 
{
public:
    class ListNode 
    {
    public:
        ListNode* Prev;
        ListNode* Next;
    };

public:
    int size;
    ListNode* Begine;
    ListNode* End;

    List() 
        : Begine(new ListNode()),
        End(new ListNode())
    {
        Begine->Next = End;
        End->Prev = Begine;
    }

    void Push_Front()
    {
        ListNode* NewNode = new ListNode();
        this->Begine->Next = NewNode;
    }

    void Push_Back() 
    {
        ListNode* NewNode = new ListNode();
        this->End->Prev = NewNode;
    }

};

//std::list<Player*> PlayerList0;
//std::list<Player*> PlayerList1;

void MemFunc()
{
    // (코드 데이터 힙) 스택 중
    // 쓰레드가 공유    
    

    // 1. 스택영역에서 처리가 가능하다면 가장 우선시해서 생각해보자.
    // 2. 같은 데이터 및 힙영역 사용하지 않을 방법이 있다면 좋다.
    //    즉 각 쓰레드마다 각자의 데이터영역 메모리 및 힙영역 메모리를 사용한다음
    //    나중에 모아서 결과만 받아볼수 있는 구조를 만들수 있는지 생각해보세요.

    // 3. 다 안된다 => 락

    // 스택은 공유하지 않습니다.
    std::list<Player*> PlayerList0;

    // 쓰레드는 기본적으로 데이터영역의 메모리는 공유한다.

    for (size_t i = 0; i < 1000000; i++)
    {
        PlayerList.push_back(new Player());
    }
}

int main()
{
    // Window는 interlock함수를 통해서 
    // _InterlockedAn

    std::thread NewThread0 = std::thread(MemFunc);
    std::thread NewThread1 = std::thread(MemFunc);

    NewThread0.join();
    NewThread1.join();

    std::cout << TestNumber;
}
