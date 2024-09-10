// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_ModifyStatusMax.h"

#include "StatusSystem/StatusComponent.h"


// Sets default values for this component's properties
UCardEffect_ModifyStatusMax::UCardEffect_ModifyStatusMax()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCardEffect_ModifyStatusMax::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_ModifyStatusMax::ResolveCardEffect(AActor* TargetActor)
{
	if (!TargetComponent->IsChildOf(UStatusComponent::StaticClass()))
	{
		return false;
	}

	UActorComponent* FoundTargetComponent = TargetActor->GetComponentByClass(TargetComponent);
	if (IsValid(FoundTargetComponent))
	{
		UStatusComponent* StatusComponent = Cast<UStatusComponent>(FoundTargetComponent);
		if (StatusComponent->bMaxAble)
		{
			int32 CalculateValue = StatusComponent->MaxValue + EffectValue;
			int32 NewValue = FMath::Max(CalculateValue, 0);
			StatusComponent->SetStatusMax(NewValue);
			return true;
		}
	}

	return false;
}
