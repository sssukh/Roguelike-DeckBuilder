﻿#include "CardSystem/CardEffects/Story/CardEffect_Story_CardPlayersTrade.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "Libraries/AssetPath.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"
#include "Utilities/CosLog.h"


// Sets default values for this component's properties
UCardEffect_Story_CardPlayersTrade::UCardEffect_Story_CardPlayersTrade()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_StoryEncounters(*AssetPath::DataTable::DT_StoryEncounters);
	if (DT_StoryEncounters.Succeeded())
	{
		CommonEncounter.DataTable = DT_StoryEncounters.Object;
		CommonEncounter.RowName = FName(*FString(TEXT("CardPlayers_a1")));

		EpicEncounter.DataTable = DT_StoryEncounters.Object;
		EpicEncounter.RowName = FName(*FString(TEXT("CardPlayers_a2")));

		TrashEncounter.DataTable = DT_StoryEncounters.Object;
		TrashEncounter.RowName = FName(*FString(TEXT("CardPlayers_a3")));
	}
	else
	{
		COS_LOG_ERROR(TEXT("DT_StoryEncounters 를 로드하지 못했습니다."));
	}
}


void UCardEffect_Story_CardPlayersTrade::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Story_CardPlayersTrade::ResolveCardEffect(AActor* TargetActor)
{
	ACardBase* TargetCard = Cast<ACardBase>(TargetActor);
	if (!TargetCard)
	{
		return false;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		return false;
	}

	if (TargetCard->Rarity == CosGameTags::Rarity_Common || TargetCard->Rarity == CosGameTags::Rarity_Rare)
	{
		IInterface_StoryEncounter::Execute_InitializeStoryEncounter(CardPlayer, CommonEncounter, false);
	}
	else if (TargetCard->Rarity == CosGameTags::Rarity_Epic || TargetCard->Rarity == CosGameTags::Rarity_Legendary)
	{
		IInterface_StoryEncounter::Execute_InitializeStoryEncounter(CardPlayer, EpicEncounter, false);
	}
	else
	{
		IInterface_StoryEncounter::Execute_InitializeStoryEncounter(CardPlayer, TrashEncounter, false);
	}

	return true;
}
