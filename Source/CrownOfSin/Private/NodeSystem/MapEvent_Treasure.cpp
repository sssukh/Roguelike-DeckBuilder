// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeSystem/MapEvent_Treasure.h"


// Sets default values for this component's properties
UMapEvent_Treasure::UMapEvent_Treasure()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	// ...
}

FGameplayTagContainer UMapEvent_Treasure::GetEncounterTags(UDataTable* DataTable, FName RowName)
{
	FStoryEncounter* StoryEncounter = DataTable->FindRow<FStoryEncounter>(RowName,"FStoryEncounter in Treasure");

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
	// 	AActor* Card = GetWorld()->SpawnActor(UCard::StaticClass(),)
}



