// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSystem/CardEffects/CardEffect_DrawSpecificCard.h"

#include "CardSystem/CardBase.h"
#include "CardSystem/CardPlayer.h"
#include "CardSystem/Piles/PileComponent.h"
#include "CardSystem/Piles/PileHandComponent.h"
#include "Libraries/FunctionLibrary_Singletons.h"


// Sets default values for this component's properties
UCardEffect_DrawSpecificCard::UCardEffect_DrawSpecificCard()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bTargeted = false;
}


void UCardEffect_DrawSpecificCard::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

bool UCardEffect_DrawSpecificCard::ResolveCardEffect(AActor* TargetActor)
{
	if (EffectValue <= 0)
		return true;

	ACardBase* TargetCard = Cast<ACardBase>(TargetActor);
	if (!TargetCard)
		return false;

	ACardPlayer* CardPlayer = UFunctionLibrary_Singletons::GetCardPlayer(this);
	if (!CardPlayer)
	{
		return false;
	}

	UPileHandComponent* PileHandComponent = Cast<UPileHandComponent>(CardPlayer->GetComponentByClass(UPileHandComponent::StaticClass()));
	if (!PileHandComponent)
	{
		return false;
	}

	if (TargetComponent->IsChildOf(UPileComponent::StaticClass()))
	{
		ACardBase* DrawnCard;
		PileHandComponent->DrawCard(TargetCard,*TargetComponent,DrawnCard);
	}
	else
	{
		ACardBase* DrawnCard;
		PileHandComponent->DrawCard(TargetCard,UPileComponent::StaticClass(),DrawnCard);
	}

	return true;
}
