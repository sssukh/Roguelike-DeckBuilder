// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeSystem/MapEvent_Story.h"

#include "CardSystem/CardPlayer.h"
#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UMapEvent_Story::UMapEvent_Story()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

FGameplayTagContainer UMapEvent_Story::GetEncounterTags(const FDataTableRowHandle& InStoryEncounter)
{
	FStoryEncounter StoryEncounter = *InStoryEncounter.DataTable->FindRow<FStoryEncounter>(InStoryEncounter.RowName,TEXT("Story Encounter in MapEvent_Story"));
	return StoryEncounter.GameplayTags;
}

void UMapEvent_Story::RunMapEvent(FDataTableRowHandle EventData)
{
	// 카드플레이어 가져와서 Interface 상속 확인
	ACardPlayer* CardPlayer = Cast<ACardPlayer>(UGameplayStatics::GetActorOfClass(this, ACardPlayer::StaticClass()));
	if(!CardPlayer->GetClass()->ImplementsInterface(UInterface_StoryEncounter::StaticClass()))
	{
		COS_SCREEN(TEXT("카드 플레이어가 UInterface_StoryEncounter를 상속받지 않았습니다"));
		return;
	}

	// 전달받은 이벤트가 유효하지 않으면 갖고있는 FillerEncounter를 전달 (초기값 채워져 있음)
	if(!IsValid(EventData.DataTable))
	{
		IInterface_StoryEncounter::Execute_InitializeStoryEncounter(CardPlayer,FillerEncounter,true);
		return;
	}

	// 게임 인스턴스 가져와서 Interface 상속 확인
	UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(this);
	if(!gameInstance->GetClass()->ImplementsInterface(UInterface_CardGameInstance::StaticClass()))
	{
		COS_SCREEN(TEXT("게임 인스턴스가 UInterface_CardGameInstance를 상속받지 않았습니다"));
		return;
	}

	// DonestoryEncounter에 현재 이벤트 추가
	// 지나간 스토리 목록에 추가
	IInterface_CardGameInstance::Execute_AddDoneStoryEncounterToInstance(gameInstance,EventData);

	// 전달받은 이벤트 전달
	IInterface_StoryEncounter::Execute_InitializeStoryEncounter(CardPlayer,EventData,true);

}



