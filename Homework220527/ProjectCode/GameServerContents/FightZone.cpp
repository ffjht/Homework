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
	//�� �ð����� üũ...... ���Ͱ� �� �׾� �ִٸ�....
	//�׾� �ִ� ���� ��Ȱ.. ����....�ᱹ �� ���ƾ�..
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