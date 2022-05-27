
#include "ThreadHandlerObjectDestroyMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "../../Play/ChatWindow.h"
#include "../../UnrealClient.h"
#include "../../Play/ClientMonster.h"
#include "../../Play/PlayGameMode.h"
#include "../../Global/ClientBlueprintFunctionLibrary.h"
#include "../../Play/ObjectMessageComponent.h"



void ThreadHandlerObjectDestroyMessage::Start()
{
	APlayGameMode* PGameMode = Cast<APlayGameMode>(UGameplayStatics::GetGameMode(World_));

	if (nullptr == PGameMode
		|| false == PGameMode->IsValidLowLevel())
	{
		UE_LOG(ClientLog, Error, TEXT("%S(%u) > GameMode Is nullptr"), __FUNCTION__, __LINE__);
		return;
	}

	//if (false == PGameMode->IsRegist(Message_->ObjectID))
	//{
	//	FTransform Transform = FTransform(Message_->Data.Pos);
	//	// ���������� ���忡 ����� ���������ִ� �Լ��� �����ؼ� ȣ���Ҽ� �ִ�.
	//	AClientMonster* NewMonster = World_->SpawnActorDeferred<AClientMonster>(Monster.Get(), Transform);
	//	// ������ �ְ� ���� ������� �� �������ش���
	//	NewMonster->FinishSpawning(Transform);
	//	NewMonster->SetId(Message_->Data.ObjectIndex);
	//	NewMonster->SetObjectType(EGameObjectType::Monster);
	//	PGameMode->RegistObject(Message_->Data.ObjectIndex, EGameObjectType::Monster, NewMonster);
	//	//PGameMode->ObjectPushMessage(Message_->Data.ObjectIndex, Message_);
	//	NewMonster->ObjectInit();
	//	return;
	//}

	
	PGameMode->ObjectPushMessage(Message_->ObjectID, Message_);
}

