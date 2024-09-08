// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_StoreTargetAsPayload.h"

#include "CardSystem/CardPlayer.h"
#include "Core/PayloadHolderComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UCardEffect_StoreTargetAsPayload::UCardEffect_StoreTargetAsPayload()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void UCardEffect_StoreTargetAsPayload::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_StoreTargetAsPayload::ResolveCardEffect(AActor* TargetActor)
{
	if (Identifier.IsEmpty())
	{
		return false;
	}

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->PayloadHolderComponent->AddPayload(Identifier, TargetActor);

	return true;
}
