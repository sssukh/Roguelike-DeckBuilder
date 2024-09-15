// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusSystem/Status_Armor.h"

#include "CardSystem/CardEffects/CardEffectComponent.h"
#include "Core/DispatcherHubComponent.h"
#include "Interfaces/Interface_CardTarget.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UStatus_Armor::UStatus_Armor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatus_Armor::BeginPlay()
{
	Super::BeginPlay();

	FGameplayTagContainer Tags;
	Tags.AddTag(CosGameTags::Event_IncomingAttack);
	Tags.AddTag(CosGameTags::Event_TurnStart);

	UDispatcherHubComponent* OwnersDispatcherHub;
	if (GetOwnersDispatcherHub(OwnersDispatcherHub))
		OwnersDispatcherHub->BindMultipleEventsToHub(this, Tags);
}

void UStatus_Armor::RunEvent_Implementation(const FGameplayTag& EventTag, UObject* CallingObject, bool bIsGlobal,
                                            UObject* PayLoad, const FGameplayTagContainer& CallTags)
{
	if (EventTag == CosGameTags::Event_TurnStart)
	{
		SetStatusValue(0, false, false, nullptr);
	}
	else if (EventTag == CosGameTags::Event_IncomingAttack)
	{
		UCardEffectComponent* CardEffectComponent = Cast<UCardEffectComponent>(CallingObject);

		OldArmor = StatusValue;

		SubtractStatusValue(CardEffectComponent->EffectValue, false, false, nullptr);

		CardEffectComponent->EffectValue = FMath::Max(CardEffectComponent->EffectValue - OldArmor, 0);

		if (CardEffectComponent->EffectValue == 0)
		{
			if (GetOwner()->GetClass()->ImplementsInterface(UInterface_CardTarget::StaticClass()))
			{
				IInterface_CardTarget::Execute_GetPuppet(GetOwner());

				//TODO
				// Action_SplashText 구현 필요
			}
		}
	}
}
