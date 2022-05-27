#include "PreCompile.h"
#include "GameServerTime.h"
#include "GameServerDebug.h"
#include <string>

GameServerTime::GameServerTime() 
{
}

GameServerTime::~GameServerTime() 
{
}



void GameServerTime::Reset()
{
	Current_ = Timer_.now();
	Prev_ = Timer_.now();
}

float GameServerTime::Update()
{
	Current_ = Timer_.now();
	DoubleDeltaTime = std::chrono::duration<double>(Current_ - Prev_).count();
	//DoubleDeltaTime = std::chrono::duration<double, std::nano>(Current_ - Prev_).count();
	//long long t = std::chrono::duration_cast<std::chrono::nanoseconds>(Current_ - Prev_).count();
	//GameServerDebug::Log(LOGTYPE::LOGTYPE_INFO, std::to_string(t));

	Prev_ = Current_;

	DeltaTime = static_cast<float>(DoubleDeltaTime);
	
	return DeltaTime;
}