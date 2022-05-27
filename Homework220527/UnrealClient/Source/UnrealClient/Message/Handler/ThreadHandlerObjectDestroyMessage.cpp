
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
	//	// 최종적으로 월드에 등록을 결정시켜주는 함수를 지연해서 호출할수 있다.
	//	AClientMonster* NewMonster = World_->SpawnActorDeferred<AClientMonster>(Monster.Get(), Transform);
	//	// 세팅해 주고 싶은 내용들을 다 세팅해준다음
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

