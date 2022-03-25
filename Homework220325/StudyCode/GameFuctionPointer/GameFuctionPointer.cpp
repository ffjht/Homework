// GameFuctionPointer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <functional>

void gFunc() 
{

}

class A 
{
public:
    int hp;

public:
    void mFunc(/*A* this*/)
    {
        // this->hp;
    }

};

class Button
{
public:
    std::function<void()> EventFunction;
    // void (T::*ClickEventFuncPtr)();

public:
    void ClickEvent() 
    {
        if (nullptr == EventFunction)
        {
            return;
        }

        EventFunction();
    }
};

//class MenuButton : public Button
//{
//public:
//    virtual void ClickEvent() override
//    {
//
//    }
//};

void MenuFunc() 
{
    std::cout << "메뉴창이 열립니다" << std::endl;
}

class Skill 
{
public:
    void Use(int _Index) 
    {
        std::cout << "플레이어가 " << _Index << "스킬을 사용합니다." << std::endl;
    }

};

class Player 
{
public:
    Skill PlayerSkill[6];

public:
    void SkillUse(int _Index) 
    {
        // std::cout << "플레이어가 스킬을 사용합니다." << std::endl;
        PlayerSkill[_Index].Use(_Index);
    }

};

void PlayerSkillFunc()
{
    // std::cout << "플레이어가 공격합니다" << std::endl;
}


int main()
{
    void(*FunctionPtr)() = gFunc;
    FunctionPtr();


    void(A::*MemberFunctionPtr)() = &A::mFunc;

    A NewA;

    // 
    (NewA.*MemberFunctionPtr)();

    {
        Button MenuButton;
        MenuButton.EventFunction = std::bind(MenuFunc);
        MenuButton.ClickEvent();
    }

    {
        Player NewPlayer;

        int a = 0;

        Button PlayerAttButton0;
        PlayerAttButton0.EventFunction = std::bind(&Player::SkillUse, &NewPlayer, a);
        PlayerAttButton0.ClickEvent();


        Button PlayerAttButton1;
        PlayerAttButton1.EventFunction = std::bind(&Player::SkillUse, &NewPlayer, 1);
        PlayerAttButton1.ClickEvent();



        Button PlayerAttButton2;
        PlayerAttButton2.EventFunction = std::bind(&Player::SkillUse, &NewPlayer, 2);
        PlayerAttButton2.ClickEvent();



        Button PlayerAttButton3;
        PlayerAttButton3.EventFunction = std::bind(&Player::SkillUse, &NewPlayer, 3);
        PlayerAttButton3.ClickEvent();



        Button PlayerAttButton4;
        PlayerAttButton4.EventFunction = std::bind(&Player::SkillUse, &NewPlayer, 4);
        PlayerAttButton4.ClickEvent();

    }

    {
        Player NewPlayer;

        std::function<void(int)> NewFunc;

        NewFunc = std::bind(&Player::SkillUse, &NewPlayer, std::placeholders::_1);

        NewFunc(10);

    }

}
