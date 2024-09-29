// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeSystem/MapEvent_Treasure.h"

#include "CardSystem/CardBase.h"
#include "Core/GlobalDispatcherHub.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/AssetPath.h"
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

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Cards_Rewards(*AssetPath::DataTable::DT_Cards_Rewards);
	if (DT_Cards_Rewards.Succeeded())
	{
		RewardCardData.DataTable = DT_Cards_Rewards.Object;
		RewardCardData.RowName = FName(TEXT("TreasureChest"));
	}
	else
	{
		COS_LOG_SCREEN_ERROR(TEXT("DT_Cards_Rewards 를 로드하지 못했습니다."));
	}
}

FGameplayTagContainer UMapEvent_Treasure::GetEncounterTags(const FDataTableRowHandle& EncounterTags)
{
	FStoryEncounter* StoryEncounter = EncounterTags.DataTable->FindRow<FStoryEncounter>(
		EncounterTags.RowName, "FStoryEncounter in Treasure");

	if (!StoryEncounter)
	{
		// 에러 처리
		return FGameplayTagContainer();
	}
	return StoryEncounter->GameplayTags;
}

void UMapEvent_Treasure::RunMapEvent(FDataTableRowHandle EventData)
{
	
	COS_IF_CHECK_VOID(!RewardCardData.IsNull(),TEXT("RewardCardData를 설정해주세요!!"));
	
	ACardBase* Card = GetWorld()->SpawnActorDeferred<ACardBase>(ACardBase::StaticClass(), FTransform::Identity, nullptr,
	                                                            nullptr,
	                                                            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Card->CardDataDeck = UFunctionLibrary_Card::MakeCardStructFromCardData(RewardCardData);
	Card->FinishSpawning(FTransform::Identity);

	TArray<AActor*> Targets;
	Card->AttemptUseCard(Targets, false, false, false);

	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
	IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance, "", true);
	UFunctionLibrary_Event::BindEventToGlobalDispatcherHub(this, CosGameTags::Event_CloseRewardScreen);
}

void UMapEvent_Treasure::RunEvent(FGameplayTag EventTag, UObject* CallingObject, bool bGlobal, UObject* Payload,
                                  FGameplayTagContainer CallTags)
{
	if (EventTag != CosGameTags::Event_CloseRewardScreen)
	{
		return;
	}

	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
	if (!gameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return;
	}
	IInterface_CardGameInstance::Execute_AttemptSaveGame(gameInstance, "", true);

	// TODO:UnbindEventToGlobalDispatcherHub() 구현 필요

	UFunctionLibrary_Event::UnBindEventFromGlobalDispatcherHub(this, CosGameTags::Event_CloseRewardScreen);
}
