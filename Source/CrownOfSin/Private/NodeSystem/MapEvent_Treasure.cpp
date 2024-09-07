// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeSystem/MapEvent_Treasure.h"

#include "CardSystem/CardBase.h"
#include "Core/GlobalDispatcherHub.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Card.h"
#include "Libraries/FunctionLibrary_Event.h"
#include "Libraries/FunctionLibrary_Utility.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UMapEvent_Treasure::UMapEvent_Treasure()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	// ...
}

FGameplayTagContainer UMapEvent_Treasure::GetEncounterTags(FDataTableRowHandle EncounterTags)
{
	FStoryEncounter* StoryEncounter = EncounterTags.DataTable->FindRow<FStoryEncounter>(EncounterTags.RowName,"FStoryEncounter in Treasure");

	if(!StoryEncounter)
	{
		// 에러 처리
		return FGameplayTagContainer();
	}
	return StoryEncounter->GameplayTags;
}

void UMapEvent_Treasure::RunMapEvent(FDataTableRowHandle EventData)
{
	// BP_Card 액터 생성
	ACardBase* Card = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(),FTransform::Identity,nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if(!Card)
	{
		COS_LOG_SCREEN(TEXT("MapEvent_Treasure : ACardBase 액터가 생성되지 않습니다."));
		return;
	}
	Card->CardDataDeck = UFunctionLibrary_Card::MakeCardStructFromCardData(RewardCardData);
	Card->FinishSpawning(FTransform::Identity);

	TArray<AActor*> Targets; 
	Card->AttemptUseCard(Targets,false,false,false);

	
	
	
	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
	if(!gameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return;
	}
	IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance,"",true);

	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this,CosGameTags::Event_CloseRewardScreen);
}

void UMapEvent_Treasure::RunEvent(FGameplayTag EventTag, UObject* CallingObject, bool bGlobal, UObject* Payload,
	FGameplayTagContainer CallTags)
{
	if(EventTag != CosGameTags::Event_CloseRewardScreen)
	{
		return;
	}

	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
	if(!gameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_LOG_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return;
	}
	IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance,"",true);

	// TODO:UnbindEventToGlobalDispatcherHub() 구현 필요

	UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(this,CosGameTags::Event_CloseRewardScreen);

}



