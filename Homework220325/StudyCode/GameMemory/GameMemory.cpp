// GameMemory.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

// 함수는 코드영역에 존재하고
// 객체의 메모리에 포함되지 않는다.

class TestClassParent
{
	// 64비트면 8바이트
	// 32비트면 4바이트가 됩니다.
	// 가상함수 테이블이생깁니다.
public:
	virtual void Func() {
		std::cout << "TestClassParent" << std::endl;
	}
};


class TestClass : public TestClassParent
{
	// 64비트면 8바이트
	// 32비트면 4바이트가 됩니다.
	// 오버라이드 해야하는 상황때문에

public:
	void Func() override 
	{
		std::cout << "TestClass" << std::endl;
	}
};

void Print() 
{
	std::cout << "PrintFunc" << std::endl;
}

class TestThis 
{
public:
	void Func() 
	{
		// 내부에서 this를 사용하지 않으므로 터지지 않는다.

		std::cout << "TestThis PrintFunc" << std::endl;
	}
};

int main()
{
	{
		TestThis* NewPtr = nullptr;

		NewPtr->Func();
	}

	{
		TestClassParent* Ptr = nullptr;
		// Ptr->가상함수테이블[0]()

		// Ptr->this->가상함수테이블[0]()
		// Ptr->Func();
	}

	{
		TestClass* Ptr = nullptr;
		// Ptr->Func();
	}


	TestClass NewTestObject;
	void (TestClass::*PtrMemberFunc)();



	//void (*PtrFunc)();
	//PtrFunc = Print;

	//PtrFunc();

	//// 가상함수테이블은 맴버함수 함수포인터이다.
	//void (TestClass::*PtrMemberFunc)();

	//PtrMemberFunc = &TestClass::Func;

	//TestClass NewFunc;

	//PtrMemberFunc();

	// 스트룹스투룹 아저씨(c++ 만든 아저씨)가
	// 메모리가 없으면 포인터이나 

	//int TestValue;
	//int* Ptr = &TestValue;
	//Ptr += 1; // Ptr 주소값 + sizeof(int);

	//TestClassParent* NewTestClass = new TestClass();

	//NewTestClass->Func();

	//// 다를게 없죠?
	//sizeof(TestClass); // 크기는?

}
