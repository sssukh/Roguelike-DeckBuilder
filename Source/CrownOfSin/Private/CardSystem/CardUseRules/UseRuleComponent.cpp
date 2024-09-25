// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardUseRules/UseRuleComponent.h"

#include "CardSystem/CardBase.h"
#include "Core/CosEnumStruct.h"


// Sets default values for this component's properties
UUseRuleComponent::UUseRuleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUseRuleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUseRuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UUseRuleComponent::CheckIfUseAllowed(FUseRule UseRuleData, FString& FailMessage)
{
	return true;
}

bool UUseRuleComponent::ResolveUseConsequence(FUseRule UseRuleData)
{
	return true;
}

void UUseRuleComponent::InitializeUseRule()
{
	ParentCard = Cast<ACardBase>(GetOwner());
}

