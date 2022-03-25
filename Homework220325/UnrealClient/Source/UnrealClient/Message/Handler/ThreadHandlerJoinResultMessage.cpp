#include "ThreadHandlerJoinResultMessage.h"
#include "../../Global/ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"

//


#include <CoreMinimal.h>

ThreadHandlerJoinResultMessage::ThreadHandlerJoinResultMessage(std::shared_ptr<JoinResultMessage> _JoinResultMessage)
	: JoinResultMessage_(_JoinResultMessage)
{
	// UGameplayStatics::
}

void ThreadHandlerJoinResultMessage::Init(UClientGameInstance* _Inst, UWorld* _World)
{
	Inst_ = _Inst;
	World_ = _World;
}


void ThreadHandlerJoinResultMessage::Start()
{
	if(EGameServerCode::JoinError == JoinResultMessage_->Code)
	{
		//UE_LOG(ClientLog, Error, TEXT("%S(%u) > JOIN BUTTON"), __FUNCTION__, __LINE__);
		UE_LOG(LogTemp, Error, TEXT("JOIN ERROR"));
	}

	// ·Î±×ÀÎ ui
}

