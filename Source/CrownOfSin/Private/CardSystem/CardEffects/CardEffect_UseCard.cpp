// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_UseCard.h"

#include "CardSystem/CardBase.h"


// Sets default values for this component's properties
UCardEffect_UseCard::UCardEffect_UseCard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCardEffect_UseCard::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_UseCard::ResolveCardEffect(AActor* TargetActor)
{
	ACardBase* TargetCard = Cast<ACardBase>(TargetActor);
	if (!TargetCard)
		return false;

	TArray<AActor*> EmptyArray;
	return TargetCard->AttemptUseCard(EmptyArray, true, true, false);
}
