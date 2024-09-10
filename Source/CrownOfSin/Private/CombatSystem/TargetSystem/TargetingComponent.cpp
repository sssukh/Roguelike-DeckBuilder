// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent.h"

#include "CardSystem/CardBase.h"
#include "Core/CosEnumStruct.h"
#include "Core/GameplayTagComponent.h"
#include "Core/MinionBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTargetingComponent::UTargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UTargetingComponent::FindValidTargets(TArray<AActor*>& SpecifiedTargets, const FCardEffect& CardEffect, ACardBase* Card,
                                           bool bPreview, TArray<AActor*>& ValidTargets)
{
	ValidTargets = TArray<AActor*>();
	return true;
}

TArray<AActor*> UTargetingComponent::FindAllMinionsWithMatchingTags(FGameplayTagContainer GameplayTags)
{
	TArray<AActor*> MatchingTargets;
	
	TArray<AActor*> FoundMinions;

	UGameplayStatics::GetAllActorsWithTag(this,FName(TEXT("Minion")),FoundMinions);

	for (AActor* FoundMinion : FoundMinions)
	{
		UGameplayTagComponent* GameplayTagComponentFound = Cast<UGameplayTagComponent>(FoundMinion->GetComponentByClass(UGameplayTagComponent::StaticClass()));

		if (GameplayTagComponentFound && GameplayTags.HasAnyExact(GameplayTagComponentFound->GameplayTags))
		{
			MatchingTargets.Add(FoundMinion);
		}
	}

	return MatchingTargets;
}

void UTargetingComponent::FindMinionTracks(ACardBase* Card, AMinionTrack*& OwnerTrack, AMinionTrack*& OpposingTrack)
{
	AMinionBase* Minion = Cast<AMinionBase>(Card->GetOwner());

	if(Minion)
	{
		OwnerTrack = Minion->MinionTrack;
		if(Minion->MinionTrack->IsValidLowLevel())
		{
			OpposingTrack = Minion->MinionTrack->OpposingTrack;
		}
	}
}

bool UTargetingComponent::GetAllMinionsOnOpposingTrack(ACardBase* Card, TArray<AMinionBase*>& Minions)
{
	Minions.Reset();
	
	AMinionTrack* OwnerTrack = nullptr;
	AMinionTrack* OpposingTrack = nullptr;
	FindMinionTracks(Card,OwnerTrack,OpposingTrack);

	if(OpposingTrack->IsValidLowLevel()&&OpposingTrack->Minions.Num()>0)
	{
		Minions =OpposingTrack->Minions;
		return true;
	}
	
	return false;
}





