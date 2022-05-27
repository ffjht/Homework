#include "PreCompile.h"
#include "FightZone.h"

FightZone::FightZone() 
{
}

FightZone::~FightZone() 
{
}


void FightZone::UserUpdate()
{
	//매 시간마다 체크...... 몬스터가 다 죽어 있다면....
	//죽어 있는 몬스터 부활.. 리젠....결국 다 돌아야..
	//
	if (0 == GetAiActors().size())
	{
		CreateActor<Monster>();
	}

	// CreateActor()
}

bool FightZone::Init() 
{
	return true;
}