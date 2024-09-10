// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_ChangeNodeMap.h"

#include "Interfaces/Interface_CardGameInstance.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UCardEffect_ChangeNodeMap::UCardEffect_ChangeNodeMap()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_ChangeNodeMap::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_ChangeNodeMap::ResolveCardEffect(AActor* TargetActor)
{
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance)
		return false;

	IInterface_CardGameInstance::Execute_ChangeNodeMapInInstance(CardGameInstance, Identifier);
	return true;
}
