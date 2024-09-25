// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardUseRules/UseRule_StatCost.h"

#include "CardSystem/CardPlayer.h"
#include "Core/CosEnumStruct.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "StatusSystem/StatusComponent.h"


// Sets default values for this component's properties
UUseRule_StatCost::UUseRule_StatCost()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUseRule_StatCost::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUseRule_StatCost::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UUseRule_StatCost::CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UStatusComponent* StatusComponent = Cast<UStatusComponent>(CardPlayer->GetComponentByClass(UseRuleData.Status));

	if(!IsValid(StatusComponent))
	{
		return false;
	}

	if(StatusComponent->StatusValue>=UseRuleData.Cost)
	{
		return true;
	}

	FailMessage = FString(TEXT("Not Enough ")).Append(StatusComponent->FriendlyName.ToString());

	return false;
}

bool UUseRule_StatCost::ResolveUseConsequence(FUseRule UseRuleData)
{
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	CardPlayer->SubtractFromStatus_Implementation(UseRuleData.Status,UseRuleData.Cost,true,nullptr);

	return true;
}

