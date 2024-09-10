// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_OpenNodeMap.h"

#include "Interfaces/Interface_CardGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UCardEffect_OpenNodeMap::UCardEffect_OpenNodeMap()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_OpenNodeMap::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_OpenNodeMap::ResolveCardEffect(AActor* TargetActor)
{
	UGameInstance* CardGameInstance = UFunctionLibrary_Singletons::GetCardGameInstance(this);
	if (!CardGameInstance)
		return false;


	if (Identifier.IsEmpty())
	{
		FString CurrentNodeMap = IInterface_CardGameInstance::Execute_GetCurrentNodeMapFromInstance(CardGameInstance);
		UGameplayStatics::OpenLevel(this, FName(*CurrentNodeMap));
	}
	else
	{
		IInterface_CardGameInstance::Execute_ChangeNodeMapInInstance(CardGameInstance, Identifier);
		UGameplayStatics::OpenLevel(this, FName(*Identifier));
	}

	return true;
}
