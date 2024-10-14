// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatSystem/TargetSystem/TargetingComponent_PayloadWithIdentifier.h"

#include "CardSystem/CardPlayer.h"
#include "Core/PayloadHolderComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"
#include "Utilities/CosGameplayTags.h"


// Sets default values for this component's properties
UTargetingComponent_PayloadWithIdentifier::UTargetingComponent_PayloadWithIdentifier()
{
	
}

bool UTargetingComponent_PayloadWithIdentifier::FindValidTargets(TArray<AActor*>& SpecifiedTargets,
                                                                 const FCardEffect& CardEffect, AEffectActor* Card, bool bPreview, TArray<AActor*>& OutValidTargets)
{
	TArray<AActor*> ValidTargets;
	
	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);

	UObject* Payload = nullptr;

	if(CardPlayer->PayloadHolderComponent->AccessPayload(CardEffect.Identifier, !CardEffect.GameplayTags.HasTagExact(CosGameTags::Flag_DoNotClear),Payload))
	{
		if(AActor* Target = Cast<AActor>(Payload))
		{
			ValidTargets.Add(Target);

			OutValidTargets = ValidTargets;
			return true;
		}
	}

	return false;
}



