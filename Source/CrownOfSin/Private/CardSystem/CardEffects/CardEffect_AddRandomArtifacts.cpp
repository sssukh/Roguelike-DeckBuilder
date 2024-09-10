// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_AddRandomArtifacts.h"

#include "CardSystem/CardPlayer.h"
#include "CardSystem/ChanceManagerComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UCardEffect_AddRandomArtifacts::UCardEffect_AddRandomArtifacts()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


void UCardEffect_AddRandomArtifacts::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_AddRandomArtifacts::ResolveCardEffect(AActor* TargetActor)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
		return false;

	int32 Amount = FMath::Max(EffectValue, 1);
	TArray<FStatusData> PickedArtifacts;
	CardPlayer->ChanceManagerComponent->GetRandomArtifactsByDefaultRarityWeights(Amount, PickedArtifacts);

	for (const FStatusData& PickedArtifact : PickedArtifacts)
	{
		IInterface_CardTarget::Execute_AddToStatus(CardPlayer, PickedArtifact.StatusClass, PickedArtifact.Value, true, this);
	}

	return true;
}
