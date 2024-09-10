﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_Summon.h"

#include "CardSystem/CardBase.h"
#include "Core/MinionBase.h"


// Sets default values for this component's properties
UCardEffect_Summon::UCardEffect_Summon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_Summon::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_Summon::ResolveCardEffect(AActor* TargetActor)
{
	AMinionBase* MinionBase = Cast<AMinionBase>(ParentCard->GetOwner());
	if (!MinionBase)
	{
		return false;
	}

	FMinion* FoundMinion = UsedData.DataTable->FindRow<FMinion>(UsedData.RowName,TEXT(""));
	if (!FoundMinion)
	{
		return false;
	}

	int32 LastIndex = FMath::Max(0, EffectValue);
	for (int i = 0; i < LastIndex; ++i)
	{
	}

	return true;
}
