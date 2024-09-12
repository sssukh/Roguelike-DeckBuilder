

#include "CardSystem/Piles/PileExhaustComponent.h"

#include "Utilities/CosGameplayTags.h"


UPileExhaustComponent::UPileExhaustComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	FriendlyName = FText::FromString(FString(TEXT("Exhaust Pile")));
	PileTag = CosGameTags::Pile_Exhaust;
	Tooltip.RowName = FName(TEXT("ExhaustPile"));
}


void UPileExhaustComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



